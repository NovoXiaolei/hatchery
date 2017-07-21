#include "utils.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "log.h"

char *getpwd(char *buf, size_t size){
    return NULL;
}

size_t strcat2(char **dst_out, ...){
    size_t len = 0, len_sub;
    va_list argp;
    char *src;
    char *dst = NULL, *dst_p=NULL;
    *dst_out = NULL;

    va_start(argp, dst_out);
    for(;;){
        if (( src = va_arg(argp, char*)) == NULL) break;
        len+=strlen(src);
    }
    va_end(argp);
    if(len==0) return 0;
    dst = (char*)malloc(sizeof(char)*(len+1));
    if(dst==NULL) return -1;
    dst_p = dst;

    va_start(argp, dst_out);
    for(;;){
        if((src=va_arg(argp, char*))==NULL) break;
        len_sub = strlen(src);
        memcpy(dst_p, src, len_sub);
        dst_p += len_sub;
    }
    va_end(argp);
    *dst_p ='\0';
    *dst_out = dst;
    return len;
}

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
    lua_getglobal(L, "daemon");

    pCon->ip = lua_tolstring(L, -4, NULL);
    pCon->port = (int)lua_tointeger(L, -3);
    pCon->log_file = lua_tolstring(L, -2, NULL);
    pCon->daemon = lua_tolstring(L, -1, NULL);

    lua_close(L);
    return 0;
}


int write_to_file(const char *pchFilePath, const char* buffer){
    FILE *fp = fopen(pchFilePath, "w+");
    if (fp == NULL)
        perror("fp is invalid due to");

    fwrite(buffer, sizeof(char), sizeof(buffer), fp);
    fclose(fp);
    return 0;
}


