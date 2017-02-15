#include <stdio.h>
#include "mysql_utils.h"

#define BUF_SIZE 1024

int main(int argc, char **argv){
    MYSQL mysql;
    if(!db_mysql_connect(&mysql)){
        printf("Failed to connect to database : Error: %s\n",
                mysql_error(&mysql));
    } 

    char *buf = (char*)malloc(BUF_SIZE);
    SQL_ROW *rows = db_mysql_fmt(&mysql, buf, BUF_SIZE, "%s %s %s %s", "insert into", "user", "(name, password, uid)","values ('test', '111111', '1001'); "); 
    if(rows){
        db_print_rows(rows);
    }
    db_free_rows(rows);

    db_mysql_close(&mysql);
    return 0;
}

