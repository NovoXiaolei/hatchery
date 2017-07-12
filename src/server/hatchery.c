#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include "../common/common.h"
#include "../utils/log.h"
SESSION_TRACKER = 1;

int main(int argc, char *argv[])
{

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
