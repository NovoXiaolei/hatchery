#include <errno.h>
#include "log.h"
#include "utils.h"
FILE *fp ;
static int SESSION_TRACKER = 0; //Keeps track of session
static char* LOG_FILE = NULL;

void set_log_file(char *log_file){
    LOG_FILE = log_file;
}

char* print_time()
{
    int size = 0;
    time_t t;
    char *buf;

    t=time(NULL); /* get current calendar time */

    char *timestr = asctime( localtime(&t) );
    timestr[strlen(timestr) - 1] = 0;  //Getting rid of \n

    size = strlen(timestr)+ 1 + 2; //Additional +2 for square braces
    buf = (char*)malloc(size);

    memset(buf, 0x0, size);
    snprintf(buf,size,"[%s]", timestr);

    return buf;
}

void log_print(char* filename, int line, char *fmt,...)
{
    va_list list;
    char *p, *r;
    int e;


    char *work_dir = get_current_dir_name();
    char *log_path = NULL;
    size_t len;
    len = strcat2(&log_path, work_dir, "/logger/", LOG_FILE, NULL);
    if (log_path==NULL)
        return;
    printf("log_path == %s\n", log_path);

    if(SESSION_TRACKER > 0)
        fp = fopen (log_path, "a+");
    else
        fp = fopen (log_path, "w+");

    if (fp == NULL){
        perror("fp is null due to");
        //printf("fp is null, error = %s\n", (char*)strerror(errno));
    }

    fprintf(fp,"%s ",print_time());
    fprintf(fp,"[%s][line: %d] ",filename,line);
    va_start( list, fmt );

    for ( p = fmt ; *p ; ++p )
    {
        if ( *p != '%' )//If simple string
        {
            fputc( *p,fp );
        }
        else
        {
            switch ( *++p )
            {
                /* string */
                case 's':
                    {
                        r = va_arg( list, char * );

                        fprintf(fp,"%s", r);
                        continue;
                    }

                    /* integer */
                case 'd':
                    {
                        e = va_arg( list, int );

                        fprintf(fp,"%d", e);
                        continue;
                    }

                default:
                    fputc( *p, fp );
            }
        }
    }
    va_end( list );
    fputc( '\n', fp );
    SESSION_TRACKER++;
    fclose(fp);
}
