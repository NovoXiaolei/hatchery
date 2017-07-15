#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include "../common/common.h"
#include "../utils/log.h"

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
    if( fp == NULL)
        return -1;

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *string = malloc(fsize+1);
    fread(string, fsize, 1, fp);
    fclose(fp);
    string[fsize]=0;
    if(luaL_loadbuffer(L, string, strlen(string), "line") || lua_pcall(L, 0, 0,0))
        LOG_PRINT("cannot run config.file:%s", lua_tostring(L, -1));

    lua_getglobal(L, "ip");
    lua_getglobal(L, "port");
    lua_getglobal(L, "log_file");
    lua_getglobal(L, "log_path");

    pCon->ip = lua_tolstring(L, -4, NULL);
    pCon->port = lua_tointeger(L, -3);
    pCon->log_file = lua_tolstring(L, -2, NULL);
    pCon->log_path = lua_tolstring(L, -1, NULL);

    LOG_PRINT("ip = %s\n",pCon->ip);
    LOG_PRINT("port = %d\n",pCon->port);
    LOG_PRINT("log_file = %s\n",pCon->log_file);
    LOG_PRINT("log_path = %s\n",pCon->log_path);

    lua_close(L);
    return 0;
}

int main(int argc, char *argv[])
{
    config conf;
    int ret = load_config("src/etc/config_server.lua", &conf);
    return 0;

    signal(SIGPIPE, SIG_IGN);

    int serv_sock;
    struct sockaddr_in serv_adr;

    if (argc !=2 ) {
        LOG_PRINT("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    int option;
    socklen_t optlen;
    optlen = sizeof(option);
    option = TRUE;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    ret = bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
    if(-1==ret){
        error_handling("listen error");
    }


    return 0;
}
