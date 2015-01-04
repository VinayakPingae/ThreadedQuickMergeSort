#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "common.h"
 
int str_is_decimal_num(char * str)
{
    int ret = 1;
    char * p = NULL;
    
    if (NULL == str) {
        printf("error: null input\n");
        return 0;
    }

    p = str;
    while (*p) {
        if (*p < '0' || *p > '9') {
            ret = 0;
            break;
        }

        p += 1;
    }

    return ret;
}

int is_regular_file(char * file_path)
{
    struct stat file_stat;

    if (-1 == stat(file_path, &file_stat)) {
        return 0;
    }

    if (S_ISREG(file_stat.st_mode)) {
        return 1;
    }

    return 0;
}

int number_of_lines(FILE * fp)
{
    int n = 0;
    size_t len = 0;
    char * line = NULL;
    
    if (NULL == fp) {
        EXIT_ERROR(ERROR, "null pointer!\n");
    }

    fseek(fp, 0, SEEK_SET);

    while (getline(&line, &len, fp) != -1) {
        n++;
    }
    
    if (line) {
        free(line);
        line = NULL;
    }

    fseek(fp, 0, SEEK_SET);
    return n;
}

int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}
