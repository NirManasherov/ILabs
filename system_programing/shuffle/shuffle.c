/*******************************************************************************
* Author: Nir Manasherov
* Project: shuffle and sort
* Reviewer: 
* Date: 26.03.2023
* Version: 1.0
*******************************************************************************/
/*******************************************************************************
                                  libraries
*******************************************************************************/
#include <stddef.h> 	        /*  size_t 			                */
#include <sys/types.h>	        /*  open 		                    */
#include <sys/stat.h>	        /*  open, mode_t	                */
#include <fcntl.h>		        /*  open 		                    */
#include <stdlib.h>		        /*  EXIT_SUCCESS                    */
#include <stdio.h>		        /*  perror		                    */
#include <sys/mman.h>	        /*  mmap			                */
#include <unistd.h>		        /*  stat			                */
#include <time.h>		        /*  time			                */
#include <string.h>		        /*  strlen		                    */
#include <pthread.h>            /*  pthread_create, pthread_join    */

#include "utils.h"
#include "heap.h"
#include "search_and_sorts.h"

/*******************************************************************************
                                  structs
*******************************************************************************/  

struct sort_struct
{
	char **strings;
	size_t size;
};

struct heap_struct
{
	char **str;
	int num_of_thread;
};

/*******************************************************************************
                                  enums
*******************************************************************************/

enum {ERROR = -1, START_OF_MAPPING = 0}; 

/*******************************************************************************
                                  globals
*******************************************************************************/

size_t g_num_of_threads = 1;

char *g_dictionary_path = "./shuffled_dict.txt"; 

char *g_shuffled_dictionary_path = "./shuffled_dict.txt"; 

/*******************************************************************************
                        Static functions declerations
*******************************************************************************/

static int ShuffleInit(void); 

static int SortInit(void);

size_t CountWords(char *mapping);

void SwapPtrs(char **str1, char**str2); 

void ShuffleDictionary(char *map, char **strings_ptr, size_t num_of_words); 

size_t StrlenNewLine(char *str);

static int SortDictionary(char **word_ptr, size_t num_of_lines, char *mapping); 

static void *Sort(void *param); 

static int MergeThreadsBlocks(struct sort_struct *args, char *mapping); 
                                                                 
static int QuickSortCmpFunc(const void *ptr1, const void *ptr2);

static int HeapCmpFunc(void *ptr1, void *ptr2);

/*******************************************************************************
                        Functions implimentations
*******************************************************************************/
int main(void)
{
     RETURN_IF_BAD(SUCCESS == ShuffleInit(), "sort init fail", FAIL);
    
     RETURN_IF_BAD(SUCCESS == SortInit(), "sort init fail", FAIL);
      
     /*return SUCCESS*/
     return SUCCESS;  
}

/******************************************************************************/

static int ShuffleInit(void)
{
    int input_file_descriptor = 0;
    int output_file_descriptor = 0;
    void *in_map = NULL;
	void *out_map = NULL;
    char *map_runner = NULL; 
    struct stat stat = {0}; 
    size_t num_of_lines = 0;
    char **word_ptr_arr = NULL;
    size_t word_arr_idx = 0; 
    
    /* open dictionary file, save file descriptor*/
    /*use O_RDWR for read-write premissions*/
    input_file_descriptor = open(g_dictionary_path, O_RDWR);
    
    RETURN_IF_BAD(ERROR != input_file_descriptor, "open fail", FAIL);
    
    /* use fstat with stat struct and file descriptor*/
    RETURN_IF_BAD(SUCCESS == fstat(input_file_descriptor, &stat), "fstat fail", 
                                                                          FAIL);  
    
    /*use mmap*/
    in_map = mmap(NULL, stat.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, 
                                       input_file_descriptor, START_OF_MAPPING);
    RETURN_IF_BAD(MAP_FAILED != in_map, "mmap failed!", 
                                                   FAIL);                              
    /*initialize map_runner */ 
    map_runner = (char *)in_map; 
    
    /*count the  number of lines in the file*/
    num_of_lines = CountWords(in_map); 
    
    /*malloc string_ptr arr*/
    word_ptr_arr = (char **)malloc(sizeof(char *)*num_of_lines);
    
    RETURN_IF_BAD(NULL != word_ptr_arr, "malloc failed!" ,FAIL); 
   
    /*initialize first word*/
    word_ptr_arr[word_arr_idx] = map_runner;
    
    /*run over the file*/
    while(EOF != *map_runner)
    {
        /*if we get a new_line than copy the next place*/
        if('\n' == *map_runner)
        {
            /*increment idx of arr and runner*/
            ++word_arr_idx; 
            ++map_runner; 
            /*copy the new word for the correct place*/
            word_ptr_arr[word_arr_idx] = map_runner;
        }
        
        /*increment runner*/
        ++map_runner; 
    }
    
    /* open dictionary file, save file descriptor and*/
    /*use O_RDWR for read-write premissions  S_IRUSR + S_IWUSR is read-write 
    for user*/
    output_file_descriptor = open(g_shuffled_dictionary_path, O_CREAT | O_RDWR | 
                                                    O_TRUNC, S_IRUSR + S_IWUSR);
    
     RETURN_IF_BAD(ERROR != output_file_descriptor, "open fail", FAIL);
    
     /*resize the output file to be in the same size as dictionary*/     
     RETURN_IF_BAD(SUCCESS == ftruncate(output_file_descriptor, stat.st_size), 
                                                        "truncate fail", FAIL); 
     
     /*map the output file*/
     out_map = mmap(NULL, stat.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, 
                                       output_file_descriptor, START_OF_MAPPING);
     
     RETURN_IF_BAD(MAP_FAILED != out_map, "mmap failed!", FAIL);  
     
     /*shuffle dictionary*/
     ShuffleDictionary(out_map, word_ptr_arr, num_of_lines);
     
     /*free word pointer array*/
     free(word_ptr_arr);
     
     /*unmap and close input file*/
     RETURN_IF_BAD(SUCCESS == munmap(out_map, stat.st_size), 
                                                           "munmap fail", FAIL);
     
     RETURN_IF_BAD(SUCCESS == close(input_file_descriptor), "close fail", FAIL); 
     
     /*unmap and close output file*/
     RETURN_IF_BAD(SUCCESS == munmap(out_map, stat.st_size), 
                                                           "munmap fail", FAIL);
     
     RETURN_IF_BAD(SUCCESS == close(output_file_descriptor) ,"close fail", FAIL);  
     
     return SUCCESS;
}

/******************************************************************************/

static int SortInit(void)
{
    int input_file_descriptor = 0;
    int output_file_descriptor = 0;
    char *map_runner = NULL; 
    struct stat stat = {0}; 
    size_t num_of_lines = 0;
    void *in_map = NULL;
	void *out_map = NULL;
	char *map_end = NULL;
	char **strings_ptr = NULL;
	size_t strings_index = 0;
	
     /*open new imput file*/
     input_file_descriptor = open(g_dictionary_path, O_RDWR); 
     
     RETURN_IF_BAD(ERROR != input_file_descriptor, "open fail", FAIL);
     
     /*use fstat for file length*/
     RETURN_IF_BAD(SUCCESS == fstat(input_file_descriptor, &stat), "fstat fail", 
                                                                          FAIL);  
     
     /*map imput file*/
     in_map = mmap(NULL, stat.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE,
						   							  input_file_descriptor, 0);
     
     RETURN_IF_BAD(MAP_FAILED != in_map, "mmap failed!", FAIL);  
     
     /*init runner ptr*/
     map_runner = (char *) in_map;
     
     /*init end ptr*/
     map_end = map_runner + stat.st_size;
     
     /*get memory resorces for string_ptr*/
     strings_ptr = (char **)malloc(sizeof(char *) * num_of_lines);
     
     RETURN_IF_BAD(NULL != strings_ptr, "malloc fail", FAIL); 
     
     /*init first string*/
     strings_ptr[strings_index] = map_runner;
     map_runner++;
	 strings_index++;
	
	/*while not in the end of mapped file*/
     while (map_runner < (map_end - 1))
	 {
	 	/*if we get new line char*/
		if ('\n' == *map_runner)
		{
			/*copy to string arr*/
			strings_ptr[strings_index] = (map_runner + 1);
			strings_index++;
		}
		/*increase map runner*/
		++map_runner;
	 }
     
     /*open output file*/
     output_file_descriptor = open(g_shuffled_dictionary_path, O_RDWR | O_CREAT | 
     									   O_TRUNC, O_TRUNC, S_IRUSR + S_IWUSR);
     
     RETURN_IF_BAD(ERROR != output_file_descriptor, "open fail", FAIL); 
     
     /*resize file*/
     ftruncate(output_file_descriptor, stat.st_size); 
     
     /*map output file*/
     out_map = mmap(NULL, stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED,
													 output_file_descriptor, 0); 
     
     RETURN_IF_BAD(MAP_FAILED != out_map, "mmap failed!", FAIL); 
     
     /*sort dictionary*/
     RETURN_IF_BAD(SUCCESS == SortDictionary(strings_ptr, num_of_lines, out_map), 
                                                   "SortDictionary fail", FAIL); 
     
     /*free string arr*/
     free(strings_ptr);
     
     /*unmap and close input file*/
     RETURN_IF_BAD(SUCCESS == munmap(in_map, stat.st_size), 
                                                           "munmap fail", FAIL);
   
     RETURN_IF_BAD(SUCCESS == close(input_file_descriptor), "close fail", FAIL);
     /*unmap and close output file*/
     RETURN_IF_BAD(SUCCESS == munmap(out_map, stat.st_size), 
     "munmap fail", FAIL);
     
     RETURN_IF_BAD(SUCCESS == close(output_file_descriptor), "close fail", FAIL); 
     
     return SUCCESS;  

}

/******************************************************************************/

size_t CountWords(char *mapping)
{
	size_t counter = 0;

	/*while not reach EOF*/
	while (EOF != *mapping)
	{
		/*if reached end of line*/
		if ('\n' == *mapping)
		{
			/*increase counter*/
			++counter;
		}
		/*increease mapping pointer*/
		++mapping;
	}
	
	/*return counter*/
	return counter;
}

/******************************************************************************/

void SwapPtrs(char **str1, char**str2)
{
	/*if str1 and str2 doesnt point to the same place*/
	if(str1 != str2)
	{
		/*make XOR SwapPtrs*/
		str1 = (char **)(((size_t)*str1) ^ ((size_t)*str2)); 
		str2 = (char **)(((size_t)*str1) ^ ((size_t)*str2));
		str1 = (char **)(((size_t)*str1) ^ ((size_t)*str2));
	}
}

/******************************************************************************/

void ShuffleDictionary(char *map, char **strings_ptr, size_t num_of_words)
{
	size_t i = 0;
	
	srand(time(NULL));

	/*foe every row*/
	for (i = 0; i < num_of_words; i++)
	{
		/*SwapPtrs str_ptr with a random char*/
		SwapPtrs(strings_ptr + i, strings_ptr + (rand() % num_of_words));
	}
	
	/*for every row*/
	for (i = 0; i < num_of_words; i++)
	{
		/*traverse by words*/
		size_t len = StrlenNewLine(strings_ptr[i]) + 1;
		/*copy words*/
		memcpy(map, strings_ptr[i], len);
		map += len;
	}
}

/******************************************************************************/

size_t StrlenNewLine(char *str)
{
	size_t counter = 0;
	
	/*while not reached EOL*/
	while ('\n' != *str)
	{
		/*increase counter and pointer*/
		counter++;
		str++;
	}
	
	/*return counter*/
	return counter;
}

/******************************************************************************/

static int SortDictionary(char **word_ptr, size_t num_of_lines, char *mapping)
{
	pthread_t *tid = NULL;
	struct sort_struct *args = NULL;
	size_t i = 0;
	size_t thread_block_size = (num_of_lines / g_num_of_threads) + 1;
    
    /*make tid by allocating resorces*/
	tid = (pthread_t *)malloc(g_num_of_threads * sizeof(pthread_t));
	/*make args by allocating resorces*/
	args = (struct sort_struct *)malloc(g_num_of_threads * sizeof(struct sort_struct));
    
    /*make threads for g_num_of_threads*/
	for (i = 0; i < g_num_of_threads; i++)
	{
	    /*init args for thread "block"*/
		args[i].strings = word_ptr + (i * thread_block_size);
		args[i].size = (i == (g_num_of_threads - 1)) ?
						num_of_lines :
						thread_block_size;
		num_of_lines -= thread_block_size;
		
		/*create threads and send them to sort routine*/
		RETURN_IF_BAD(SUCCESS == pthread_create(tid + i, NULL, Sort, args + i), 
		                                           "pthread_create fail", FAIL);
		
	}
    
    /*for every thread*/
	for (i = 0; i < g_num_of_threads; i++)
	{
	    /*join threads*/
		RETURN_IF_BAD(SUCCESS == pthread_join(tid[i], NULL), "pthread_join fail", 
		                                                                  FAIL);
		
	}
	
	/*free tid*/
	free(tid);
	tid = NULL;
    
    /*merge thread blocks*/
	MergeThreadsBlocks(args, mapping);
	
	/*free args*/
	free(args);
	args = NULL; 
	
	return SUCCESS; 
}

/******************************************************************************/

static void *Sort(void *param)
{
    /*extract args from params*/
	struct sort_struct *args = (struct sort_struct *) param;

    /*sort with one of the sorting algorithms*/
	QuickSort(args->strings, args->size, sizeof(char *), QuickSortCmpFunc);

    /*return NULL*/
	return NULL;
}

/******************************************************************************/

static int MergeThreadsBlocks(struct sort_struct *args, char *mapping)
{
	size_t i = 0;
	struct heap_struct *data_arr = NULL;
	struct heap_struct *data = NULL; 
	heap_ty *min_heap = HeapCreate(HeapCmpFunc);
	size_t len = 0; 
    
    /*take resorces for data_arr*/
	data_arr = (struct heap_struct *)malloc(g_num_of_threads *    
	                                                  sizeof(struct heap_struct));

    RETURN_IF_BAD(NULL != data_arr, "malloc fail", FAIL); 
    
    /*for every thread*/
	for (i = 0; i < g_num_of_threads; i++)
	{
	    /*init data_arr struct*/
		data_arr[i].str = args[i].strings;
		data_arr[i].num_of_thread = i;
		
		/*heap push*/
	    RETURN_IF_BAD(SUCCESS == HeapPush(min_heap, &data_arr[i]), 
	                                                    "heap push fail", FAIL);  
	}

    /*while heap is not empty*/
	while (!HeapIsEmpty(min_heap))
	{
		/*get min data with heap peek and then heap pop*/
		data = HeapPeek(min_heap);
		
		HeapPop(min_heap);
		/*get size of word*/
		len = StrlenNewLine(*(data->str)) + 1;
		/*cpy to mapping*/
		memcpy(mapping, *(data->str), len);
		/*get to next word*/
		mapping += len;
		(data->str)++;
		
		/*if in place*/
		if (data->str < (args[data->num_of_thread].strings + 
		                                         args[data->num_of_thread].size))
		{
		    /*heap push*/ 
			RETURN_IF_BAD(SUCCESS == HeapPush(min_heap, data), "heap push fail", 
			                                                              FAIL); 
		}
	}
	
	/*set mapping pointer to NULL*/
	mapping = NULL;
	
	/*free data array*/
	free(data_arr);
	
	/*heap destroy*/
	HeapDestroy(min_heap);
	
	return SUCCESS; 
}

/******************************************************************************/

static int QuickSortCmpFunc(const void *ptr1, const void *ptr2)
{
	const unsigned char *str1 = *((const unsigned char **) ptr1);
	const unsigned char *str2 = *((const unsigned char **) ptr2);
	
	/*while str1 != str2*/
	while (*str1 == *str2)
	{
		/*if wee got new line char*/
		if ('\n' == *str1)
		{
			return 0;
		}
		
		/*increase str1 and str2*/
		str1++;
		str2++;
	}
	
	/*return sdr1 - str2*/
	return (*str1 - *str2);	
}

/******************************************************************************/

static int HeapCmpFunc(void *ptr1, void *ptr2)
{	
	const unsigned char *str1 = *((const unsigned char **) 
	                                        ((struct heap_struct *) ptr1)->str);
	const unsigned char *str2 = *((const unsigned char **) 
	                                        ((struct heap_struct *) ptr2)->str);
	
	/*while str1 != str2*/
	while (*str1 == *str2)
	{
		/*if wee got new line char*/
		if ('\n' == *str1)
		{
			return 0;
		}
		
		/*increase str1 and str2*/
		str1++;
		str2++;
	}
	
	/*return sdr1 - str2*/
	return (*str2 - *str1);	
}

/******************************************************************************/
