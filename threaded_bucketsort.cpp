/*
ECEN 4313: Concurrent Programming
Author: Elena Murray
Date: 9/30/2020
Lab 2: 

*Note modified from test.c provide
    
*/

//Library includes
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <list>
#include <cmath>

//Developer includes
#include "threaded_bucketsort.hpp"
// #include "mergesort.hpp"
#include "array_splitter.hpp"

pthread_t *threads;
pthread_mutex_t * locks; 


size_t NUM_THREADS;
size_t NUM_BUCKETS; 
pthread_barrier_t p_bar;



struct timespec start;
struct timespec end;


/*
	Allocates all required data. 
*/
void global_init()
{
	threads = (pthread_t *)malloc(NUM_THREADS * sizeof(pthread_t));
	locks = new pthread_mutex_t[NUM_BUCKETS]; 
	//inialize each lock 
	for(int i = 0; i < NUM_BUCKETS; i++){
		locks[i] = PTHREAD_MUTEX_INITIALIZER; 
	}
	pthread_barrier_init(&p_bar, NULL, NUM_THREADS);

}

/*
	Frees all allocated data. 
*/
void global_cleanup()
{
	free(threads);
	delete locks; 
	pthread_barrier_destroy(&p_bar);
}


/*
	Bucketsort algo with locks to protect each index. 
*/
void bucketsort(std::vector<int> array, int max,std::list<int> * buckets ){
	int b;  
	for(int i = 0; i < array.size(); i++){ 
		b = floor(NUM_BUCKETS * (double)array[i]/max); 
		//protect bucket number b
		pthread_mutex_lock(&locks[b]); 
 
		// //Add to list
		buckets[b].push_back(array[i]); 
		
		pthread_mutex_unlock(&locks[b]);
	}
}

std::vector<int> concatenate(std::list<int> * buckets, int num_buckets){
	std::vector<int> data; 
	// printf("num_buckets = %d", num_buckets); 
	for(int i = 0; i < num_buckets; i++){
		// printf("\ni = %d",i); 
		if(!buckets[i].empty()){
			// printf("\nsorting bucket[%d]",i); 
			buckets[i].sort(); 
			for(std::list<int>::iterator it = buckets[i].begin(); it!= buckets[i].end(); it++){
					// printf("%d ",*it); 
					data.push_back((int)*it); 
			}
		}else{
			// printf("\nskipping bucket[%d]",i); 
		}
	}
	return data; 
}

/*
	Prints out buckets. 
*/
void printBuckets(std::list<int> * buckets, int num_buckets){
	for(int i = 0; i < num_buckets; i++){
		printf("\nbucket[%d]: ",i); 
		for(std::list<int>::iterator it = buckets[i].begin(); it!= buckets[i].end(); it++){
			printf("%d ",*it); 
		}
	}
	printf("\n"); 
}

/*
	Thread version of bucketsort. Runs bucketsort with the assigned data. 
*/
void *bucketsort_thread(void *args)
{
	size_t tid = ((struct threaded_bucketsort_args *)args)->tid; //*((size_t*)args);
	std::vector<int> array = ((struct threaded_bucketsort_args *)args)->array;
	std::list<int> * buckets = ((struct threaded_bucketsort_args *)args)->buckets;
	int max = ((struct threaded_bucketsort_args *)args)->max; 
	
	pthread_barrier_wait(&p_bar);
	if (tid == 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &start);
	}
	pthread_barrier_wait(&p_bar);

	bucketsort(array, max, buckets); 
	

	pthread_barrier_wait(&p_bar);
	if (tid == 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &end);
	}

	return 0;
}

/*
	Runs the parallelized bucketsort. Creates and joins all threads and returns
	the data to main. 
*/
int run_threaded_bucketsort(int num_threads, std::vector <int> &data)
{

	NUM_THREADS = num_threads;
	if (NUM_THREADS > 150) 
	{
		printf("ERROR; too many threads\n");
		exit(-1);
	}
	//Calcualte how many buckets are required
	NUM_BUCKETS = calc_num_buckets(data, NUM_THREADS); 

	//Create buckets
	std::list<int> buckets[NUM_BUCKETS]; 
	//Global init
	global_init();

	//Split array into NUM_THREADS parts
	std::vector<std::vector<int>> split_arrays = split_vector_array(data, NUM_THREADS);

	//Find the maximum value of the array
	int MAX_VALUE = calc_max(data);
	
	struct threaded_bucketsort_args args[NUM_THREADS];
	
	//Setting up the master thread args data
	args[0].tid = 1; //i;
	args[0].array = split_arrays[0];
	args[0].buckets = buckets; 
	args[0].max = MAX_VALUE; 


	// // launch threads
	int ret;
	size_t i;

	for (i = 1; i < NUM_THREADS; i++)
	{
		args[i].tid = i + 1;
		args[i].array = split_arrays[i];
		args[i].buckets = buckets; 
		args[i].max = MAX_VALUE; 
		// printf("creating thread %zu\n",args[i].tid);
		ret = pthread_create(&threads[i], NULL, &bucketsort_thread, &args[i]);
		if (ret)
		{
			printf("ERROR; pthread_create: %d\n", ret);
			exit(-1);
		}
	}

	bucketsort_thread(&args); // master also calls mergesort_thread

	// join threads
	for (size_t i = 1; i < NUM_THREADS; i++)
	{
		ret = pthread_join(threads[i], NULL);
		if (ret)
		{
			printf("ERROR; pthread_join: %d\n", ret);
			exit(-1);
		}
		// printf("joined thread %zu\n",i+1);
	}

	//Concatenate all buckets
	data  = concatenate(buckets, NUM_BUCKETS); 

	global_cleanup();

	unsigned long long elapsed_ns;
	elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
	printf("Elapsed (ns): %llu\n", elapsed_ns);
	double elapsed_s = ((double)elapsed_ns) / 1000000000.0;
	printf("Elapsed (s): %lf\n", elapsed_s);
	return 0; 
}