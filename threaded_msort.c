/*
II.1. Program that performs a parallel merge sort using Pthreads.

Name - Vinayak Subhash Pingale		Email - vpingal1@binghamton.edu

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "common.h"
#define MAX 100

int str_is_decimal_num(char * str);
int is_regular_file(char * file_path);
int number_of_lines(FILE * fp);
int cmpfunc(const void *, const void *);

// Sturcture for catching elements of the merge sort 
typedef struct merge_sort {
       int *start_merge_array;
	   int start_element_idx;
       int  first;
	   int  last;
}MERGE_ELEMENT; 

// Sturcture for catching elements of the quick sort 
typedef struct quick_sort {
       int *start_array;
       int  starting_idx;
}QUICK_ELEMENT; 
 

// Calling the quick sort function which will be passed as a thread handler for pthread_create
void* quick_sort(void *arguments) {
    struct quick_sort *quick_sort = arguments;
	// Invoking inbuilt quick sort function
    qsort(quick_sort->start_array, quick_sort->starting_idx , sizeof(int), cmpfunc);
    pthread_exit(NULL);
    return NULL;
}
// Calling the divide sort which is nothing but a merge sort function which will be passed as a thread handler for pthread_create
void* divide_sort(void *arguments) {
		// Invoking merge sort operation
    struct merge_sort *merge_sort_variable = arguments;
    int *a = merge_sort_variable->start_merge_array;
	int starting_idx= merge_sort_variable->start_element_idx;
	int first = merge_sort_variable->first; 
	int second = merge_sort_variable->last;
	int i = starting_idx;	
	int j = starting_idx+first; // Checking for the second split
	int* sorted=malloc(sizeof(int)*(first+second));
	int l =0,m=0,k=0;
	// Merge sorting of the file.
	while(i < (first+starting_idx) && j < (starting_idx+first+second)) {
		if(a[i] < a [j]) {
			sorted[k] = a[i];
			k++;
			i++;
		} else {
			sorted[k] = a[j];
			k++;
			j++;
		}
	
	}
	while(i < (first+starting_idx)) {
			sorted[k] = a[i];
			k++;
			i++;
	}
	while(j < (starting_idx+first+second)) {
			sorted[k] = a[j];
			k++;
			j++;
	}
	k=first+second;
	m=starting_idx;
	for(l=0;l<k;l++) {
		a[m] = sorted[l];
		m++;
	}
	free(sorted);
	pthread_exit(NULL);
    return NULL;
}


int main(int argc, char * argv[])
{
    char *idata_fp = NULL, *line = NULL;
    FILE *idata_fs = NULL;
    size_t len;
    unsigned int data_size = 0;
    int split_num = 0, i = 0;
    int *data = NULL;

    /* --- handle user inputs --- */
    if (argc < 3) {
        printf("Usage: %s input_data_file split_num\n", argv[0]);
        exit(1);
    }

    if (!is_regular_file(argv[1])) {
        printf("Regular input data file %s does not exist.\n", argv[1]);
        exit(1);
    }

    if (!str_is_decimal_num(argv[2])) {
        printf("%s is not a valid split size. It should be a decimal number. \n", argv[2]);
        exit(1);
    }

    idata_fp = argv[1];
    split_num = atoi(argv[2]);

    /* --- open the input data file --- */
    idata_fs = fopen(idata_fp, "r");
    if (NULL == idata_fs) {
        EXIT_ERROR(ERROR, "fopen failed!\n");
    }

    /* --- get the number of input data, and allocate data buffer accordingly ---*/
    data_size = number_of_lines(idata_fs);
    if (data_size < split_num)
    {
        printf("data_size(%d) is smaller than split_num(%d)!\n", data_size, split_num);
        exit(1);
    }

    /* --- allocate mem for the input data --- */
    data = malloc(data_size * sizeof(*data));
    if (NULL == data) {
        EXIT_ERROR(ERROR, "malloc failed!\n");
    }

    /* --- read the numbers to sort from the input file to the data buffer ---*/
    i = 0;
    while (getline(&line, &len, idata_fs) != -1) {
        data[i] = atoi(line);
        i++;
    }

    /* --- print the input data ---*/
    printf("========== input data (data_size=%d, split_num=%d) ==========\n", data_size, split_num);
    for (i = 0; i < data_size; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");


    int size_of_splits = data_size/split_num;
    int *start_splits = malloc(sizeof(int)*split_num),start_idx = 0;
    pthread_t *threads;
    int returnError;
 	
    threads=malloc(sizeof(pthread_t)*(split_num+1));
    for(i=0 ; i<split_num-1 ;i++) {
		start_splits[i] = size_of_splits;
	}
    start_splits[i] = (data_size - (split_num-1)*size_of_splits);
	// Quick Sort operation
    printf("\n----- qsort round: %d threads needed -----\n",split_num);
    for (i = 0; i < split_num; i++) { 

         
         struct quick_sort *q_sort = malloc(sizeof(struct quick_sort));

         q_sort->start_array = &data[start_idx];
         q_sort->starting_idx = start_splits[i];

         returnError = pthread_create(&threads[i], NULL, quick_sort, q_sort);
         if (returnError){
          printf("ERROR; return code from pthread_create() is %d\n", returnError);
          exit(-1);
         }
 
         printf("qsort thread: start element idx: %d; sorting size: %d\n",start_idx,start_splits[i]);
         start_idx += start_splits[i];
         //qsort(data, start_idx, sizeof(int), cmpfunc);
    }
	for(i=0;i<split_num;i++)
	{

		pthread_join(threads[i],NULL);

	}
	//free(threads);
	threads=NULL;
    printf("\n");
    // START MERGING
	start_idx = 0;
	int last_size = 0, first_size = 0;
	int split_num_new = split_num;
	int addition_ofall = 0;
	int flag=0,count=0;
	
	while(split_num_new >= 1) {
		if(split_num_new  & 1) {
			flag=1;
		}

		addition_ofall = 0;
		if(count != 0) {
			for(i=0 ; i < split_num_new-1 ;i++) {
				start_splits[i] = 2*start_splits[i];
				addition_ofall += start_splits[i];
			}
			
		}
		count++;
		start_splits[i] = (data_size - addition_ofall);
		split_num_new = (split_num_new/2);
		threads=malloc(sizeof(pthread_t)*split_num_new);
		start_idx = 0;
		if(split_num_new != 0) {
			printf("\n----- merge round: %d threads needed ----- \n",split_num_new);
		}
		for (i = 0; i < split_num_new; i++) { 
			threads[i]=i;

			 MERGE_ELEMENT *m_sort = malloc(sizeof(MERGE_ELEMENT));
			 m_sort->start_merge_array = data;
			 m_sort->start_element_idx = start_idx;
			 first_size = start_splits[(i*2)];
		     m_sort->first = first_size;
			 last_size = start_splits[(i*2)+1];
			 m_sort->last =  last_size;
			 printf("merge thread: start element idx = %d ; 1st split size: %d ; 2nd split size: %d \n",start_idx,first_size,last_size);
			 returnError = pthread_create(&threads[i], NULL, divide_sort, m_sort);
			 //printf("%d \n",k);
		     if (returnError){
		      printf("ERROR; return code from pthread_create() is %d\n", returnError);
		      exit(-1);
		     }
			start_idx += start_splits[i*2]+start_splits[(i*2)+1];        
		}   
		int p;

		for(p=0;p<split_num_new;p++)
		{
			pthread_join(threads[p],NULL);


		}

		//printf("after join\n");
		free(threads);
		 
		//split--; 
		if(flag==1 && split_num_new != 0) {
			split_num_new = split_num_new+1;
		}
		flag=0;
	}

    /* --- print the result ---*/
	
    fprintf(stderr, "\n========== threaded merge sort result ==========\n");
    for (i = 0; i < data_size; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    /* Last thing that main() should do */
    pthread_exit(NULL);
   
    /* --- free resourses ---*/
    if (line) {
        free(line);
        line = NULL;
    }

    if (data) {
        free (data);
        data = NULL;
    }
    
    fclose(idata_fs);
    exit(0);
}




