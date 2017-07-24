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
#include "utils.h"

#define BUF_SIZE 1024

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

    set_log_file(config.log_file);

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
