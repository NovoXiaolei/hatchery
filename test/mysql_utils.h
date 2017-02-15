//Reference to: git@github.com:lattera/mysql.git

#ifndef _MYSQL_UTILS_H
#define _MYSQL_UTILS_H


#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define PROG_NAME "hatchery"

typedef struct sql_col{
    char *name;
    char *data;
    struct sql_col *next;
} SQL_COL;

typedef struct sql_row{
    SQL_COL *cols;
    struct sql_row *next;
} SQL_ROW;

MYSQL *db_mysql_connect(MYSQL *mysql){
    mysql_init(mysql);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, PROG_NAME);
    
    return mysql_real_connect(mysql, "127.0.0.1", "root", "1qaz2wsxE",
        "room1007", 0, NULL, 0);    
}

void db_mysql_close(MYSQL *mysql){
    mysql_close(mysql);
} 

SQL_ROW *db_mysql_run(MYSQL *mysql, const char* statement){
    SQL_ROW *rows=NULL, *row = NULL;
    SQL_COL *col = NULL;
    MYSQL_ROW mysqlrow;
    MYSQL_RES *res;
    MYSQL_FIELD *fields;
    unsigned int i;
    unsigned int num_fields;

    int ret = mysql_query(mysql, statement);
    if(ret !=0){
        printf("op failed %s \n", mysql_error(mysql));
        return NULL;
    }
    res = mysql_store_result(mysql);
    
    if(!(res)){
        goto end;
    }

    num_fields = mysql_num_fields(res);
    fields = mysql_fetch_fields(res);

    while((mysqlrow = mysql_fetch_row(res))){
        if (!(row)){
            rows = calloc(1, sizeof(SQL_ROW));
            if(!(rows))
                goto end;
            row = rows;
        }else{
            row->next = calloc(1, sizeof(SQL_ROW));
            if(!(row->next))
                goto end;
            row = row->next;
        }

        for(i = 0; i < num_fields; i++){
            if(!(row->cols)){
                row->cols = calloc(1, sizeof(SQL_COL));
                if( !(row->cols) ) 
                    goto end;

                col = row->cols;        
            } else {
                col->next = calloc(1, sizeof(SQL_COL));

                if( !(col->next) )
                    goto end;

                col = col->next;
            }

            if( fields[i].name )
                col->name = strdup(fields[i].name);

            if(mysqlrow[i])
                col->data = strdup(mysqlrow[i]);
        }
    }
end:
    if(res)
        mysql_free_result(res);
    
    return rows;
}

SQL_ROW *db_mysql_fmt(MYSQL *mysql, char *buf, size_t bufsz, char *fmt, ...){
    va_list ap;

    va_start(ap, fmt);
    int n = vsnprintf(buf, bufsz, fmt, ap);
    va_end(ap);
    
    printf("sql fmt = %s %d \n", buf, n);

    return db_mysql_run(mysql, buf);
}

char *db_get_column(SQL_ROW *row, char *name){
    SQL_COL *col;
    if(!(row))
        return NULL;

    for(col = row->cols; col; col = col->next)
        if((col->name))
            if(!strcmp(col->name, name))
                return col->data;

    return NULL;
}

void db_free_rows(SQL_ROW *rows){
    SQL_ROW *cur_row, *next_row;
    SQL_COL *cur_col, *next_col;
    cur_row = rows;

    while(cur_row){
        next_row = cur_row->next;

        cur_col = cur_row->cols;
        while(cur_col){
            next_col = cur_col->next;

            if(cur_col->name)
                free(cur_col->name);
            if(cur_col->data)
                free(cur_col->data);
            free(cur_col);
            cur_col = next_col;
        }

        free(cur_row);
        cur_row = next_row;

    }
}

void db_print_rows(SQL_ROW *rows){
    SQL_ROW *row;
    SQL_COL *col;
    unsigned int i = 0;

    row = rows;
    while(row){
        fprintf(stderr, "[*] Row #%d\n", i++);
        col = row->cols;
        while(col){
            if((col->name) && (col->data))
                fprintf(stderr, "[*] \t%s = %s \n", col->name, col->data);
            col = col->next;
        }
        row = row->next;
    }
}
#endif
