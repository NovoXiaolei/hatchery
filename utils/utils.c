#include "utils.h"
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
