/*
II.2.

Name - Vinayak Subhash Pingale		Email - vpingal1@binghamton.edu

*/


#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define MAXSTRINGSZ 4096

extern char **environ;

//static char buf[MAXSTRINGSZ];
static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t strerrorkey;
static char *buffer;
static void destructor (void *buff_destoy) {
	free(buff_destoy);
}
static void buffer_alloc() {
	pthread_key_create(&strerrorkey, destructor); // Referring to the slide pthread_2 page number 31 and 32
}


char * mygetenv(const char *name)
{
	
    int  i, len;
	len = strlen(name);
		for (i = 0; environ[i] != NULL; i++) 
		{
		    if ((strncmp(name, environ[i], len) == 0) && (environ[i][len] == '=')) 
			{	
				 pthread_once(&once, buffer_alloc);
                 pthread_setspecific(strerrorkey, malloc(MAXSTRINGSZ));
           		 buffer= pthread_getspecific(strerrorkey);
            	 strncpy(buffer, &environ[i][len+1], MAXSTRINGSZ-1);
				 buffer[MAXSTRINGSZ-1] = '\0';
		         return buffer;
			} 
		}
		
	
	return NULL;
}

int main ()
{
       char *input_shell;       
       input_shell = mygetenv("SHELL");
		// Accepting the environment varibale and printing out the values present
        printf ("SHELL --->> : %s\n",input_shell);
       return 0;
}
