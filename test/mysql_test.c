#include <stdio.h>
#include "../utils/mysql_utils.h"

#define BUF_SIZE 1024

int main(int argc, char **argv){
    MYSQL mysql;
    if(!db_mysql_connect(&mysql)){
        printf("Failed to connect to database : Error: %s\n",
                mysql_error(&mysql));
    } 

    char *buf = (char*)malloc(BUF_SIZE);
    SQL_ROW *rows = NULL;
    rows = db_mysql_select(&mysql, buf, BUF_SIZE, "%s", "select * from user;"); 
    if(rows != NULL){
        db_print_rows(rows);
    }
    else
    {
        printf("Failed to op to database : Error: %s \n", mysql_error(&mysql));
    }
    db_free_rows(rows);

    db_mysql_close(&mysql);
    return 0;
}

