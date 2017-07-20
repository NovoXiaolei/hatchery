#ifndef CLIENT_H
#define CLIENT_H

#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "common.h"
#include "log.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#define BUF_SIZE 1024

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
    lua_getglobal(L, "log_path");

    pCon->ip = lua_tolstring(L, -4, NULL);
    pCon->port = (int)lua_tointeger(L, -3);
    pCon->log_file = lua_tolstring(L, -2, NULL);
    pCon->log_path = lua_tolstring(L, -1, NULL);

    lua_close(L);
    return 0;
}

int main(int argc, char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len;

    if (argv[1] == NULL)
        LOG_PRINT("config file is invalid");

    config config;
    char *config_fie = argv[1];
    int ret = load_config(argv[1], &config);

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock==-1)
        LOG_PRINT("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(config.port);

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
        LOG_PRINT("connect() error");

    while(1){
        fputs("Input message(q to quit):", stdout);
        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message, "q\n"))
            break;

        write(sock, message, strlen(message));
        str_len = read(sock, message, BUF_SIZE-1);
        message[str_len]=0;
        LOG_PRINT("Message from server: %s", message);
    }

    close(sock);
    return 0;
}

#endif
