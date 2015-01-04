#include <string.h>
#include <stdlib.h>

#define MAXSTRINGSZ 4096

extern char **environ;

static char buf[MAXSTRINGSZ];

char * mygetenv(const char *name)
{
    int  i, len;

    len = strlen(name);
    for (i = 0; environ[i] != NULL; i++) 
	{
        if ((strncmp(name, environ[i], len) == 0) && (environ[i][len] == ’=’)) 
		{
            strncpy(buf, &environ[i][len+1], MAXSTRINGSZ-1);
			buf[MAXSTRINGSZ-1] = '\0';
            return buf;
		} 
	}
	
	return NULL;
}
