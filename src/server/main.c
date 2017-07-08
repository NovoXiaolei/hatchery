#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "common.h"

#define BUF_SIZE 100
#define MAX_CLNT 256
#define TRUE 1
#define FALSE 0

void *handle_clnt(void *arg);
void send_msg(char *msg, int len);
int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

int main(int argc, char *argv[])
{
    //当客户端在挂掉的情况下，如果此时服务端向客户端发送数据
    //则内核会发送一个SIGPIPE的消息给服务端，而SIGPIPE默认的行为是cut down当前进程
    //因此服务端则容易crash，一般的操作则是忽略此信号 
    signal(SIGPIPE, SIG_IGN);

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;
    int clnt_adr_sz;
    pthread_t t_id;
    if( argc!=2 ){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);        
    }
    
    //初始化锁
    pthread_mutex_init(&mutx, NULL);
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
    if(-1 == ret){
        error_handling("bind() error");
    }

    if(listen(serv_sock, 5) == -1 ){
        error_handling("listen() error");
    }

    while(1){
        clnt_adr_sz = sizeof(clnt_adr);

        //new client accept
        clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

        pthread_mutex_lock(&mutx);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutx);

        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
        //new thread go into background
        pthread_detach(t_id);
        printf("Commected client IP :%s \n", inet_ntoa(clnt_adr.sin_addr));
    }

    close(serv_sock);

    printf("Hello, hatchery\n");
    return 0;
}

void *handle_clnt(void *arg){
   int clnt_sock = *((int*)arg);
   int str_len= 0, i;
   char msg[BUF_SIZE];

   while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0){
       send_msg(msg, str_len);
   }
   pthread_mutex_lock(&mutx);
   for(i = 0;i<clnt_cnt;i++){
       if(clnt_sock == clnt_socks[i]){
           while(i++ < clnt_cnt-1)
               clnt_socks[i]=clnt_socks[i+1];
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

