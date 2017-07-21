#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <errno.h>
#include "common.h"
#include "log.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


enum{
    TRUE = 0,
    FALSE = 1
};

#define BUF_SIZE 100
#define MAX_CLNT 256

void read_childproc(int sig);
void *handle_clnt(void *arg);
void send_msg(char *msg, int len);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

int load_config(const char *pchConfig, config *pCon){
    lua_State *L = luaL_newstate();
    if (NULL == L)
        return -1;

    luaL_openlibs(L);

    FILE *fp = fopen(pchConfig, "r");
    if( fp == NULL){
        printf("%s\n",pchConfig);
        perror("open config lua failed :");
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

    pCon->ip = lua_tolstring(L, -3, NULL);
    pCon->port = (int)lua_tointeger(L, -2);
    pCon->log_file = lua_tolstring(L, -1, NULL);


    lua_close(L);
    return 0;
}

int main(int argc, char *argv[])
{
    config conf;

    printf("%s\n", argv[1]);
    if (argv[1] == NULL)
        LOG_PRINT("config file is invalid");
    char *config_file = argv[1];
    int ret = load_config(config_file, &conf);

    set_log_file(conf.log_file);
    signal(SIGPIPE, SIG_IGN);

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    unsigned int clnt_adr_sz;
    pid_t pid;
    int str_len;
    pthread_t t_id;

    pthread_mutex_init(&mutx, NULL);

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

    LOG_PRINT("server start\n");

    while(1){
        clnt_adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

        pthread_mutex_lock(&mutx);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutx);

        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
        pthread_detach(t_id);
        printf("Connected client IP:%s \n", inet_ntoa(clnt_adr.sin_addr));
    }
    close(serv_sock);
    return 0;
}

void *handle_clnt(void *arg){
    int clnt_sock = *((int*)arg);
    int str_len=0, i;
    char msg[BUF_SIZE];
    while((str_len = read(clnt_sock, msg, sizeof(msg)))!=0)
        send_msg(msg, str_len);

    pthread_mutex_lock(&mutx);
    for(i = 0;i<clnt_cnt;i++){
        if(clnt_sock == clnt_socks[i]){
            while(i++<clnt_cnt-1)
                clnt_socks[i] = clnt_socks[i+1];
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutx);
    close(clnt_sock);
    return NULL;
}

void send_msg(char *msg, int len){
    int i;
    pthread_mutex_lock(&mutx);
    for(i = 0;i<clnt_cnt;i++)
        write(clnt_socks[i], msg, len);
    pthread_mutex_unlock(&mutx);
}

void read_childproc(int sig){
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    LOG_PRINT("remove proc id :%d \n", pid);
}
