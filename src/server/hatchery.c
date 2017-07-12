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
#include "lauxlib.h"
SESSION_TRACKER = 1;

typedef struct config{
    const char* ip = NULL;
    int port = 0;
    const char* log_file = NULL;
    const char* log_path = NULL;
}config;

int load_config(const char* config, config* pCon){
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    if(luaL_loadfile(L, config) || lua_pcall(L, 0, 0,0))
        log_print(__FILE__, __LINE__, "cannot run config.file:%s", lua_tostring(L, -1));

    lua_getglobal(L, "ip");
    lua_getglobal(L, "port");
    lua_getglobal(L, "log_file");
    lua_getglobal(L, "log_path");

    pCon->ip = lua_tolstring(L, -4, NULL);
    pCon->port = lua_tointeger(L, -3);
    pCon->log_file = lua_tolstring(L, -2, NULL);
    pCon->log_path = lua_tolstring(L, -1, NULL);

    log_print(__FILE__, __LINE__, "ip = %s\n",pConf->ip);
    log_print(__FILE__, __LINE__, "port = %d\n",pConf->port);
    log_print(__FILE__, __LINE__, "log_file = %s\n",pConf->log_file);
    log_print(__FILE__, __LINE__, "log_path = %s\n",pConf->log_path);

    lua_close(L);
    return 0;
}

int main(int argc, char *argv[])
{
    config conf;
    int ret = load_config("../etc/config_server.lua", &conf);
    if(ret == 0){

    }

    signal(SIGPIE, SIG_IGN);

    int serv_sock;
    struct sockaddr_in serv_adr;

    if (argc !=2 ) {
        log_print(__FILE__, __LINE__, "Usage : %s <port>\n", argv[0]);
        exit(1)
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

    int ret = bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
    if(-1==ret){
        error_handling("listen error");
    }


    return 0;
}
