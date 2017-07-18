#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <errno.h>
#include "common.h"
#include "log.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

typedef struct Config{
    const char* ip;
    int port;
    const char* log_file;
    const char* log_path;
}config;

enum{
    TRUE = 0,
    FALSE = 1
};

#define BUF_SIZE 30

void read_childproc(int sig);

int load_config(const char *pchConfig, config *pCon){
    lua_State *L = luaL_newstate();
    if (NULL == L)
        return -1;

    luaL_openlibs(L);
    /*
       luaopen_base(L);
       luaopen_table(L);
       luaopen_io(L);
       luaopen_string(L);
       luaopen_math(L);
       */

    FILE *fp = fopen(pchConfig, "r");
    if( fp == NULL){
        perror("open config lua failed : ");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *string = (char*)malloc( (unsigned long)fsize+ 1);
    fread(string, (unsigned long)fsize, 1, fp);
    fclose(fp);
    string[fsize]=0;
    if(luaL_loadbuffer(L, string, strlen(string), "line") || lua_pcall(L, 0, 0,0)){
        LOG_PRINT("cannot run config.file:%s\n", lua_tostring(L, -1));
    }



    lua_getglobal(L, "ip");
    lua_getglobal(L, "port");
    lua_getglobal(L, "log_file");
    lua_getglobal(L, "log_path");

    pCon->ip = lua_tolstring(L, -4, NULL);
    pCon->port = (int)lua_tointeger(L, -3);
    pCon->log_file = lua_tolstring(L, -2, NULL);
    pCon->log_path = lua_tolstring(L, -1, NULL);

    LOG_PRINT("ip = %s\n",  pCon->ip);
    LOG_PRINT("port = %d\n", pCon->port);
    LOG_PRINT("log_file = %s\n", pCon->log_file);
    LOG_PRINT("log_path = %s\n", pCon->log_path);


    lua_close(L);
    return 0;
}

int main(int argc, char *argv[])
{
    config conf;
    int ret = load_config("../etc/config_server.lua", &conf);

    signal(SIGPIPE, SIG_IGN);

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    pid_t pid;
    int str_len;

    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    int state = sigaction(SIGCHLD, &act, 0);

    char buf[BUF_SIZE];
    socklen_t adr_sz;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    int option;
    socklen_t optlen;
    optlen = sizeof(option);
    option = TRUE;

    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(conf.port);

    ret = bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
    if(-1==ret){
        LOG_PRINT("bind error, errno = %d\n", errno);
    }

    ret = listen(serv_sock, 5);
    if (ret == -1 ){
        LOG_PRINT("listen error, errno=%d\n", errno);
    }

    while(1){
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if(clnt_sock==-1)
            continue;
        else
            LOG_PRINT("new client connected");
        pid = fork();
        if (pid == -1){
            close(clnt_sock);
            continue;
        }

        if(pid==0){
            //子进程中运行
            close(serv_sock);
            while((str_len==read(clnt_sock, buf, BUF_SIZE))!=0){
                write(clnt_sock, buf, str_len);
            }

            close(clnt_sock);
            LOG_PRINT("client disconnected");
            return 0;
        }
        else
            close(clnt_sock);
    }


    close(serv_sock);
    return 0;
}

void read_childproc(int sig){
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    LOG_PRINT("remove proc id :%d \n", pid);
}
