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
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

//Developer includes
#include "threaded_counter.hpp"
#include "../conc_primitives/tas_lock.hpp"
#include "../conc_primitives/ttas_lock.hpp"
#include "../conc_primitives/ticket_lock.hpp"
#include "../conc_primitives/sense_barrier.hpp"

/*************************************************
	GLOBAL VARIABLES
**************************************************/
size_t* args;
size_t NUM_THREADS;
size_t NUM_ITER; 
size_t IMP_METHOD; 
int counter;  
void * (*thread_foo)(void *); 

//Threads
pthread_t* threads;

//Barriers
pthread_barrier_t p_bar;
SenseBarrier s_bar; 

//Locks
pthread_mutex_t p_lock; 
TasLock tas_lock;  
TtasLock ttas_lock; 
TicketLock ticket_lock; 

struct timespec start, end;

/*************************************************
	FUNCTION DECLARATIONS
**************************************************/
void* thread_main(void* args); 
void* counter_pthread_lock(void* args); 
void* counter_tas_lock(void* args); 
void* counter_tas_lock(void* args); 
void* counter_ttas_lock(void* args); 
void* counter_ticket_lock(void* args); 

void* counter_pthread_bar(void* args); 
void* counter_sense_bar(void* args); 


/*************************************************
	GLOBAL INIT AND CLEANING FUNCTIONS
**************************************************/
/*
	Allocates all required data. 
*/
void global_init(){

	//Variables used in all cases
	threads = (pthread_t *)malloc(NUM_THREADS*sizeof(pthread_t)); //
	args = (size_t*)malloc(NUM_THREADS*sizeof(size_t));
	pthread_barrier_init(&p_bar, NULL, NUM_THREADS); //Needed for all the timing analysis
	counter = 0; 

	// printf("\nRunning counter with %ld threads, %ld iterations, and method = ", NUM_THREADS, NUM_ITER); 
	
	//Configure global variables based on the input type
	switch (IMP_METHOD){
		case SENSE_BAR:
			// printf("SENSE_BAR\n"); 
			thread_foo =  &counter_sense_bar; 
			s_bar.init(NUM_THREADS); 
			break;
		case PTHREAD_BAR:
			// printf("PTHREAD_BAR\n"); 
			thread_foo = &counter_pthread_bar;
			break;
		case TAS_LOCK:
			// printf("TAS_LOCK\n"); 
			thread_foo = &counter_tas_lock; 
			break;
		case TTAS_LOCK:
			// printf("TTAS_LOCK\n"); 
			thread_foo = &counter_ttas_lock;  
			break;
		case TICKET_LOCK:
			// printf("TICKET_LOCK\n"); 
			thread_foo = &counter_ticket_lock; 
			break;
		case PTHREAD_LOCK:
			// printf("PTHREAD_LOCK\n"); 
			p_lock = PTHREAD_MUTEX_INITIALIZER;
			thread_foo = &counter_pthread_lock; 
			break;

		default:
			break;
		}
}


/*
	Frees all allocated data. 
*/
void global_cleanup(){
	//Variables to free everytime
	free(threads);
	free(args);
	pthread_barrier_destroy(&p_bar);
}


void local_init(){}
void local_cleanup(){}

/*************************************************
	COUNTER FUNCTIONS
**************************************************/
/*
	Thread version of counter that uses a pthread lock to protect counter. 
*/
void* counter_pthread_lock(void* args){
	size_t tid = *((size_t*)args);
	local_init();
	pthread_barrier_wait(&p_bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&start);
	}
	pthread_barrier_wait(&p_bar);
	
	for(int i = 0; i < NUM_ITER; i++){
		pthread_mutex_lock(&p_lock); 
		counter++; 
		pthread_mutex_unlock(&p_lock);
	}

	pthread_barrier_wait(&p_bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&end);
	}
	local_cleanup();
	
	return 0;
}

/*
	Thread version of counter that uses a tas lock to protect counter. 
*/
void* counter_tas_lock(void* args){
	size_t tid = *((size_t*)args);
	local_init();
	pthread_barrier_wait(&p_bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&start);
	}
	pthread_barrier_wait(&p_bar);

	for(int i = 0; i < NUM_ITER; i++){
		tas_lock.lock(); 
		counter++; 
		tas_lock.unlock(); 
	}

	pthread_barrier_wait(&p_bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&end);
	}
	local_cleanup();
	
	return 0;
}

/*
	Thread version of counter that uses a ttas lock to protect counter.  
*/
void* counter_ttas_lock(void* args){
	size_t tid = *((size_t*)args);
	local_init();
	pthread_barrier_wait(&p_bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&start);
	}
	pthread_barrier_wait(&p_bar);

	for(int i = 0; i < NUM_ITER; i++){ 
		ttas_lock.lock(); 
		counter++; 
		ttas_lock.unlock(); 
	}

	pthread_barrier_wait(&p_bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&end);
	}
	local_cleanup();
	
	return 0;
}

/*
	Thread version of counter that uses a ticket lock to protect counter. 
*/
void* counter_ticket_lock(void* args){
	size_t tid = *((size_t*)args);
	local_init();
	pthread_barrier_wait(&p_bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&start);
	}
	pthread_barrier_wait(&p_bar);
	
	for(int i = 0; i < NUM_ITER; i++){
		ticket_lock.lock();  
		counter++; 
		ticket_lock.unlock();  
	}

	pthread_barrier_wait(&p_bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&end);
	}
	local_cleanup();
	
	return 0;
}

/*
	Thread version of counter that uses a pthread barrier to protect counter. 
*/
void* counter_pthread_bar(void* args){
	size_t tid = *((size_t*)args);
	local_init();
	pthread_barrier_wait(&p_bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&start);
	}
	pthread_barrier_wait(&p_bar);
	
	for(int i = 0; i < NUM_ITER * NUM_THREADS; i++){
		if(i%NUM_THREADS+1==tid){
			counter++; 
			assert(counter== i+1); 
		}
		pthread_barrier_wait(&p_bar);
	}


	pthread_barrier_wait(&p_bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&end);
	}
	local_cleanup();
	
	return 0;
}

/*
	Thread version of counter that uses a sense barrier to protect counter. 
*/
void* counter_sense_bar(void* args){
	size_t tid = *((size_t*)args);
	local_init();
	pthread_barrier_wait(&p_bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&start);
	}
	pthread_barrier_wait(&p_bar);
	
	for(int i = 0; i < NUM_ITER * NUM_THREADS; i++){
		if(i%NUM_THREADS+1==tid){
			counter++; 
			assert(counter== i+1); 
		}
		s_bar.wait(); 
	}

	pthread_barrier_wait(&p_bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&end);
	}
	local_cleanup();
	
	return 0;
}

/*************************************************
	THREADED COUNTER 
**************************************************/
/*
	Runs the parallelized counter. Creates and joins all threads and returns
	the data to main. 
*/
int run_threaded_counter(int num_threads, int num_iter, int imp_method, int & final_cntr){
	
	/* SETUP GLOBAL VARIABLES */
	NUM_THREADS = num_threads;
	NUM_ITER = num_iter; 
	IMP_METHOD = imp_method; 

	/* INITIALIZE THREADS, LOCKS, & BARRIERS */
	global_init();
	
	
	/* LAUNCH THREADS */
	int ret; size_t i;
  	for(i=1; i<NUM_THREADS; i++){
		args[i]=i+1;
		// printf("creating thread %zu\n",args[i]);
		ret = pthread_create(&threads[i], NULL, thread_foo, &args[i]);
		if(ret){
			printf("ERROR; pthread_create: %d\n", ret);
			exit(-1);
		}
	}
	i = 1;
	//run counter on the main/master thread
	(*thread_foo)(&i); 
	
	/* JOIN THREADS */
	for(size_t i=1; i<NUM_THREADS; i++){
		ret = pthread_join(threads[i],NULL);
		if(ret){
			printf("ERROR; pthread_join: %d\n", ret);
			exit(-1);
		}
		// printf("joined thread %zu\n",i+1);
	}
	
	/* CHECK & RETURN COUNTER */
	assert(counter == NUM_THREADS *NUM_ITER); 	//Check that counter is the right value
	
	//Return the counter
	final_cntr = counter; 
	
	/* CLEAN UP */
	global_cleanup();

	/* PRINT TIME OUT */
	printf("Timing Results:\n");
	unsigned long long elapsed_ns;
	elapsed_ns = (end.tv_sec-start.tv_sec)*1000000000 + (end.tv_nsec-start.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
	return 0; 
}