/*
ECEN 4313: Concurrent Programming
Author: Elena Murray
Date: 9/30/2020
Lab 2: 

*Note modified from test.c provide
    
*/

/*************************************************
	FILE INCLUDES
**************************************************/
//Library includes
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <list>
#include <cmath>

//Developer includes
#include "threaded_bucketsort.hpp"
#include "array_splitter.hpp"
#include "../conc_primitives/tas_lock.hpp"
#include "../conc_primitives/ttas_lock.hpp"
#include "../conc_primitives/ticket_lock.hpp"
#include "../conc_primitives/sense_barrier.hpp"

/*************************************************
	GLOBAL VARIABLES
**************************************************/
pthread_t *threads;
size_t NUM_THREADS;
size_t NUM_BUCKETS; 
size_t IMP_METHOD; 

void * (*thread_foo)(void *); 

//Barriers
pthread_barrier_t p_bar;
SenseBarrier s_bar; 

//Locks
pthread_mutex_t * p_locks; 
TasLock * tas_locks;
TtasLock * ttas_locks;
TicketLock * ticket_locks; 

struct timespec start;
struct timespec end;

/*************************************************
	FUNCTION DECLARATIONS
**************************************************/
void *bucketsort_p_lock_p_bar(void *args); 
void *bucketsort_tas_p_bar(void *args); 
void *bucketsort_ttas_p_bar(void *args); 
void *bucketsort_ticket_p_bar(void *args); 

void *bucketsort_p_lock_s_bar(void *args); 
void *bucketsort_tas_s_bar(void *args); 
void *bucketsort_ttas_s_bar(void *args); 
void *bucketsort_ticket_s_bar(void *args); 

/*************************************************
	GLOBAL INIT AND CLEANING FUNCTIONS
**************************************************/
/*
	Allocates all required data. 
*/
void global_init()
{
	threads = (pthread_t *)malloc(NUM_THREADS * sizeof(pthread_t));
	
	
	// printf("\nRunning threaded_bucketsort with %ld threads and method = ", NUM_THREADS); 

	switch (IMP_METHOD){
		case TAS_LOCK_PTHREAD_BAR:
			// printf("TAS_LOCK_PTHREAD_BAR\n");
			//Set to the right function 
			thread_foo = &bucketsort_tas_p_bar;   
			//Inialize tas_locks 
			tas_locks = new TasLock[NUM_BUCKETS]; 
			//Init pthread barrier
			pthread_barrier_init(&p_bar, NULL, NUM_THREADS);
			break;
		case TTAS_LOCK_PTHREAD_BAR:
			// printf("TTAS_LOCK_PTHREAD_BAR\n");
			//Set to the right function 
			thread_foo = &bucketsort_ttas_p_bar; 
			//Inialize ttas_locks
			ttas_locks = new TtasLock[NUM_BUCKETS]; 
			//Init pthread barrier
			pthread_barrier_init(&p_bar, NULL, NUM_THREADS);
			break;
		case TICKET_LOCK_PTHREAD_BAR:
			// printf("TICKET_LOCK_PTHREAD_BAR\n");
			//Set to the right function 
			thread_foo = &bucketsort_ticket_p_bar; 
			//Inialize ticket_locks 
			ticket_locks = new TicketLock[NUM_BUCKETS]; 
			//Init pthread barrier
			pthread_barrier_init(&p_bar, NULL, NUM_THREADS);
			break;
		case PTHREAD_LOCK_PTHREAD_BAR:
			// printf("PTHREAD_LOCK_PTHREAD_BAR\n"); 
			//Set to the right function 
			thread_foo = &bucketsort_p_lock_p_bar; 
			//Inialize p_locks
			p_locks = new pthread_mutex_t[NUM_BUCKETS]; 
			for(int i = 0; i < NUM_BUCKETS; i++){
				p_locks[i] = PTHREAD_MUTEX_INITIALIZER; 
			}
			//Init pthread barrier
			pthread_barrier_init(&p_bar, NULL, NUM_THREADS);
			break;
		case TAS_LOCK_SENSE_BAR:
			// printf("TAS_LOCK_SENSE_BAR\n"); 
			//Set to the right function 
			thread_foo = &bucketsort_tas_s_bar; 
			//Inialize tas_locks
			tas_locks = new TasLock[NUM_BUCKETS]; 
			//Init sense barrier barrier
			s_bar.init(NUM_THREADS); 
			break;
		case TTAS_LOCK_SENSE_BAR:
			// printf("TTAS_LOCK_SENSE_BAR\n"); 
			//Set to the right function 
			thread_foo = &bucketsort_ttas_s_bar; 
			//Inialize each pthread lock 
			ttas_locks = new TtasLock[NUM_BUCKETS]; 
			//Init sense barrier barrier
			s_bar.init(NUM_THREADS); 
			break;
		case TICKET_LOCK_SENSE_BAR:
			// printf("TICKET_LOCK_SENSE_BAR\n"); 
			//Set to the right function 
			thread_foo = &bucketsort_ticket_s_bar; 
			//Inialize each pthread lock 
			ticket_locks = new TicketLock[NUM_BUCKETS]; 
			//Init sense barrier barrier
			s_bar.init(NUM_THREADS); 
			break;
		case PTHREAD_LOCK_SENSE_BAR:
			// printf("PTHREAD_LOCK_SENSE_BAR\n"); 
			//Set to the right function 
			thread_foo = &bucketsort_p_lock_s_bar;  
			//Inialize each pthread lock 
			p_locks = new pthread_mutex_t[NUM_BUCKETS]; 
			for(int i = 0; i < NUM_BUCKETS; i++){
				p_locks[i] = PTHREAD_MUTEX_INITIALIZER; 
			}
			//Init sense barrier barrier
			s_bar.init(NUM_THREADS); 

		default:
			//Set to the default function
			thread_foo = &bucketsort_p_lock_p_bar; 
			//Inialize each pthread lock 
			p_locks = new pthread_mutex_t[NUM_BUCKETS]; 
			for(int i = 0; i < NUM_BUCKETS; i++){
				p_locks[i] = PTHREAD_MUTEX_INITIALIZER; 
			}
			//Init pthread barrier
			pthread_barrier_init(&p_bar, NULL, NUM_THREADS);
			break;
		}

}

/*
	Frees all allocated data. 
*/
void global_cleanup()
{
	free(threads);
	 
	
	// printf("\nCleaning counter with method = "); 
	
	switch (IMP_METHOD){
		case TAS_LOCK_PTHREAD_BAR:
			// printf("TAS_LOCK_PTHREAD_BAR\n"); 
			pthread_barrier_destroy(&p_bar);
			delete tas_locks;
			break;
		case TTAS_LOCK_PTHREAD_BAR:
			// printf("TTAS_LOCK_PTHREAD_BAR\n"); 
			pthread_barrier_destroy(&p_bar);
			delete ttas_locks;
			break;
		case TICKET_LOCK_PTHREAD_BAR:
			// printf("TICKET_LOCK_PTHREAD_BAR\n"); 
			pthread_barrier_destroy(&p_bar);
			delete ticket_locks;
			break;
		case PTHREAD_LOCK_PTHREAD_BAR:
			// printf("PTHREAD_LOCK_PTHREAD_BAR\n"); 
			delete p_locks;
			pthread_barrier_destroy(&p_bar);
			break;
		case TAS_LOCK_SENSE_BAR:
			// printf("TAS_LOCK_SENSE_BAR\n"); 
			delete tas_locks;
			break;
		case TTAS_LOCK_SENSE_BAR:
			// printf("TTAS_LOCK_SENSE_BAR\n"); 
			delete ttas_locks;
			break;
		case TICKET_LOCK_SENSE_BAR:
			// printf("TICKET_LOCK_SENSE_BAR\n"); 
			delete ticket_locks;
			break;
		case PTHREAD_LOCK_SENSE_BAR:
			// printf("PTHREAD_LOCK_SENSE_BAR\n"); 
			delete p_locks;
			break;

		default:
			pthread_barrier_destroy(&p_bar);
			delete p_locks;
			break;
		}
}

/*************************************************
	HELPER FUNCTIONS
**************************************************/
/*
	Concatenates sorted buckets. 
*/
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

/*************************************************
	BUCKETSORT FUNCTIONS
**************************************************/
/*
	Thread version of bucketsort that uses pthread locks to protect the buckets
	and pthread barriers for timing. Runs bucketsort with the assigned data. 
*/
void *bucketsort_p_lock_p_bar(void *args)
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

	int b;  
	for(int i = 0; i < array.size(); i++){ 
		b = floor(NUM_BUCKETS * (double)array[i]/max); 
		//protect bucket number b
		pthread_mutex_lock(&p_locks[b]); 
 
		// //Add to list
		buckets[b].push_back(array[i]); 
		
		pthread_mutex_unlock(&p_locks[b]);
	}
	

	pthread_barrier_wait(&p_bar);
	if (tid == 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &end);
	}

	return 0;
}

/*
	Thread version of bucketsort that uses tas locks to protect the buckets
	and pthread barriers for timing. Runs bucketsort with the assigned data. 
*/
void *bucketsort_tas_p_bar(void *args)
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

	int b;  
	for(int i = 0; i < array.size(); i++){ 
		b = floor(NUM_BUCKETS * (double)array[i]/max); 
		//protect bucket number b
		tas_locks[b].lock(); 
		//Add to list
		buckets[b].push_back(array[i]); 
		tas_locks[b].unlock(); 
	}
	

	pthread_barrier_wait(&p_bar);
	if (tid == 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &end);
	}

	return 0;
}

/*
	Thread version of bucketsort that uses ttas locks to protect the buckets
	and pthread barriers for timing. Runs bucketsort with the assigned data. 
*/
void *bucketsort_ttas_p_bar(void *args)
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

	int b;  
	for(int i = 0; i < array.size(); i++){ 
		b = floor(NUM_BUCKETS * (double)array[i]/max); 
		//protect bucket number b
		ttas_locks[b].lock(); 
		//Add to list
		buckets[b].push_back(array[i]); 
		ttas_locks[b].unlock(); 
	}
	

	pthread_barrier_wait(&p_bar);
	if (tid == 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &end);
	}

	return 0;
}

/*
	Thread version of bucketsort that uses ticket locks to protect the buckets
	and pthread barriers for timing. Runs bucketsort with the assigned data. 
*/
void *bucketsort_ticket_p_bar(void *args)
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

	int b;  
	for(int i = 0; i < array.size(); i++){ 
		b = floor(NUM_BUCKETS * (double)array[i]/max); 
		//protect bucket number b
		ticket_locks[b].lock(); 
		//Add to list
		buckets[b].push_back(array[i]); 
		ticket_locks[b].unlock(); 
	}
	

	pthread_barrier_wait(&p_bar);
	if (tid == 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &end);
	}

	return 0;
}

/*
	Thread version of bucketsort that uses pthread locks to protect the buckets
	and sense barriers for timing. Runs bucketsort with the assigned data. 
*/
void *bucketsort_p_lock_s_bar(void *args)
{
	size_t tid = ((struct threaded_bucketsort_args *)args)->tid; //*((size_t*)args);
	std::vector<int> array = ((struct threaded_bucketsort_args *)args)->array;
	std::list<int> * buckets = ((struct threaded_bucketsort_args *)args)->buckets;
	int max = ((struct threaded_bucketsort_args *)args)->max; 
	
	// pthread_barrier_wait(&p_bar);
	s_bar.wait(); 
	if (tid == 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &start);
	}
	// pthread_barrier_wait(&p_bar);
	s_bar.wait(); 

	int b;  
	for(int i = 0; i < array.size(); i++){ 
		b = floor(NUM_BUCKETS * (double)array[i]/max); 
		//protect bucket number b
		pthread_mutex_lock(&p_locks[b]); 
 
		// //Add to list
		buckets[b].push_back(array[i]); 
		
		pthread_mutex_unlock(&p_locks[b]);
	}
	
	s_bar.wait(); 
	// pthread_barrier_wait(&p_bar);
	if (tid == 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &end);
	}

	return 0;
}

/*
	Thread version of bucketsort that uses tas locks to protect the buckets
	and sense barriers for timing. Runs bucketsort with the assigned data. 
*/
void *bucketsort_tas_s_bar(void *args)
{
	size_t tid = ((struct threaded_bucketsort_args *)args)->tid; //*((size_t*)args);
	std::vector<int> array = ((struct threaded_bucketsort_args *)args)->array;
	std::list<int> * buckets = ((struct threaded_bucketsort_args *)args)->buckets;
	int max = ((struct threaded_bucketsort_args *)args)->max; 
	
	// pthread_barrier_wait(&p_bar);
	s_bar.wait(); 
	if (tid == 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &start);
	}
	s_bar.wait(); 

	int b;  
	for(int i = 0; i < array.size(); i++){ 
		b = floor(NUM_BUCKETS * (double)array[i]/max); 
		//protect bucket number b
		tas_locks[b].lock(); 
		//Add to list
		buckets[b].push_back(array[i]); 
		tas_locks[b].unlock();  
	}
	
	s_bar.wait(); 
	if (tid == 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &end);
	}

	return 0;
}

/*
	Thread version of bucketsort that uses ttas locks to protect the buckets
	and sense barriers for timing. Runs bucketsort with the assigned data. 
*/
void *bucketsort_ttas_s_bar(void *args)
{
	size_t tid = ((struct threaded_bucketsort_args *)args)->tid; //*((size_t*)args);
	std::vector<int> array = ((struct threaded_bucketsort_args *)args)->array;
	std::list<int> * buckets = ((struct threaded_bucketsort_args *)args)->buckets;
	int max = ((struct threaded_bucketsort_args *)args)->max; 
	
	// pthread_barrier_wait(&p_bar);
	s_bar.wait(); 
	if (tid == 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &start);
	}
	s_bar.wait(); 

	int b;  
	for(int i = 0; i < array.size(); i++){ 
		b = floor(NUM_BUCKETS * (double)array[i]/max); 
		//protect bucket number b
		ttas_locks[b].lock(); 
		//Add to list
		buckets[b].push_back(array[i]); 
		ttas_locks[b].unlock();  
	}
	
	s_bar.wait(); 
	if (tid == 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &end);
	}

	return 0;
}

/*
	Thread version of bucketsort that uses ticket locks to protect the buckets
	and sense barriers for timing. Runs bucketsort with the assigned data. 
*/
void *bucketsort_ticket_s_bar(void *args)
{
	size_t tid = ((struct threaded_bucketsort_args *)args)->tid; //*((size_t*)args);
	std::vector<int> array = ((struct threaded_bucketsort_args *)args)->array;
	std::list<int> * buckets = ((struct threaded_bucketsort_args *)args)->buckets;
	int max = ((struct threaded_bucketsort_args *)args)->max; 
	
	// pthread_barrier_wait(&p_bar);
	s_bar.wait(); 
	if (tid == 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &start);
	}
	s_bar.wait(); 

	int b;  
	for(int i = 0; i < array.size(); i++){ 
		b = floor(NUM_BUCKETS * (double)array[i]/max); 
		//protect bucket number b
		ticket_locks[b].lock(); 
		//Add to list
		buckets[b].push_back(array[i]); 
		ticket_locks[b].unlock();  
	}
	
	s_bar.wait(); 
	if (tid == 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &end);
	}

	return 0;
}

/*************************************************
	THREADED BUCKETSORT
**************************************************/
/*
	Runs the parallelized bucketsort with the correct timing barriers and lock implementations.
	Creates and joins all threads and returns the data to main. 
*/
int run_threaded_bucketsort(int num_threads, int imp_method, std::vector <int> &data)
{
	/* SETUP GLOBAL VARIABLES */
	NUM_THREADS = num_threads;
	IMP_METHOD = imp_method; 

	if (NUM_THREADS > 150) 	//Limit the number of threads
	{
		printf("ERROR; too many threads\n");
		exit(-1);
	}
	NUM_BUCKETS = calc_num_buckets(data, NUM_THREADS); //Calcualte how many buckets are required
	
	/* INITIALIZE THREADS, LOCKS, & BARRIERS */
	global_init(); 

	/* SETUP BUCKETS */
	std::list<int> buckets[NUM_BUCKETS]; //Create buckets
	std::vector<std::vector<int>> split_arrays = split_vector_array(data, NUM_THREADS); //Split array into NUM_THREADS parts
	
	/* SETUP ARGUMENNTS FOR THREADS */
	int MAX_VALUE = calc_max(data); //Find the maximum value of the array
	struct threaded_bucketsort_args args[NUM_THREADS];
	//Setting up the master thread args data
	args[0].tid = 1; 
	args[0].array = split_arrays[0];
	args[0].buckets = buckets; 
	args[0].max = MAX_VALUE; 


	/* LAUNCH THREADS */
	int ret;
	size_t i;
	for (i = 1; i < NUM_THREADS; i++)
	{
		args[i].tid = i + 1;
		args[i].array = split_arrays[i];
		args[i].buckets = buckets; 
		args[i].max = MAX_VALUE; 
		// printf("creating thread %zu\n",args[i].tid);
		ret = pthread_create(&threads[i], NULL, thread_foo, &args[i]);
		if (ret)
		{
			printf("ERROR; pthread_create: %d\n", ret);
			exit(-1);
		}
	}

	//run buckestort on the main/master thread
	(*thread_foo)(&args); 

	/* JOIN THREADS */
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

	/* CONCATENATE ALL BUCKETS */
	data  = concatenate(buckets, NUM_BUCKETS); 

	/* CLEAN UP */
	global_cleanup();

	/* PRINT TIME OUT */
	printf("Timing Results:\n");
	unsigned long long elapsed_ns;
	elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
	printf("Elapsed (ns): %llu\n", elapsed_ns);
	double elapsed_s = ((double)elapsed_ns) / 1000000000.0;
	printf("Elapsed (s): %lf\n", elapsed_s);
	return 0; 
}


