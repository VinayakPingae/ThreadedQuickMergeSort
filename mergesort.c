#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "common.h"
#define MAX 100

int str_is_decimal_num(char * str);
int is_regular_file(char * file_path);
int number_of_lines(FILE * fp);
int cmpfunc(const void *, const void *);
int sorted[200];

typedef struct merge_sort {
       int *start_merge_array;
	   int start_element_idx;
       int  first;
	   int  last;
}MERGE_ELEMENT; 

typedef struct quick_sort {
       int *start_array;
       int  starting_idx;
}QUICK_ELEMENT; 
 


int quick_sort(void *arguments) {
    struct quick_sort *quick_sort = arguments;
    qsort(quick_sort->start_array, quick_sort->starting_idx , sizeof(int), cmpfunc);
    pthread_exit(NULL);
    return 0;
}

int divide_sort(void *arguments) {
    struct merge_sort *merge_sort_variable = arguments;
    merge(merge_sort_variable->start_merge_array, merge_sort_variable->first, merge_sort_variable->start_merge_array, merge_sort_variable->last,sorted);
 	pthread_exit(NULL);
    return 0;
}

void merge(int a[], int m, int b[], int n, int sorted[]) {
  int i, j, k;
 
  j = k = 0;
 
  for (i = 0; i < m + n;) {
    if (j < m && k < n) {
      if (a[j] < b[k]) {
        sorted[i] = a[j];
        j++;
      }
      else {
        sorted[i] = b[k];
        k++;
      }
      i++;
    }
    else if (j == m) {
      for (; i < m + n;) {
        sorted[i] = b[k];
        k++;
        i++;
      }
    }
    else {
      for (; i < m + n;) {
        sorted[i] = a[j];
        j++;
        i++;
      }
    }
  }
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


	// Add your implementation here ...
    int size_of_splits = data_size/split_num;
    int *start_splits = malloc(sizeof(int)*split_num),start_idx = 0;
    pthread_t threads[split_num];
    int returnError;
 	
    
    for(i=0 ; i<split_num-1 ;i++) {
		start_splits[i] = size_of_splits;
	}
    start_splits[i] = (data_size - (split_num-1)*size_of_splits);

    printf("----- qsort round: %d threads needed -----\n",split_num);
    for (i = 0; i < split_num; i++) { 

         
         struct quick_sort *q_sort = malloc(sizeof(quick_sort));

         q_sort->start_array = data;
         q_sort->starting_idx = start_idx;

         returnError = pthread_create(&threads[i], NULL, quick_sort, q_sort);
         if (returnError){
          printf("ERROR; return code from pthread_create() is %d\n", returnError);
          exit(-1);
         }
 
         printf("qsort thread: start element idx: %d; sorting size: %d\n",start_idx,start_splits[i]);
         start_idx += start_splits[i];
         //qsort(data, start_idx, sizeof(int), cmpfunc);
    }
    printf("\n");
    // START MERGING
	start_idx = 0;
	int last_size = 0, first_size = 0;
	int size_of_merge = (split_num/2);	
	for (i = 0; i <= size_of_merge; i++) { 

		 MERGE_ELEMENT *m_sort = malloc(sizeof(MERGE_ELEMENT));

         m_sort->start_merge_array = data;
		 first_size += start_idx + start_splits[(i*2)];
         m_sort->first = first_size;
	     printf("first_size: %d\t",first_size);
		 last_size += start_splits[i*2] + start_splits[(i*2)+1];
		 m_sort->last =  last_size;
		 printf("last_size %d\n",last_size);
		 returnError = pthread_create(&threads[i], NULL, divide_sort, m_sort);
         if (returnError){
          printf("ERROR; return code from pthread_create() is %d\n", returnError);
          exit(-1);
         }
		//start_idx += start_splits[i+1];        
	}       

    /* --- print the result ---*/
    fprintf(stderr, "========== threaded merge sort result ==========\n");
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




