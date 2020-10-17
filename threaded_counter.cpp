#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

//Developer includes
#include "threaded_counter.hpp"
#include "conc_primitives/tas_lock.hpp"

//Global Variables
size_t* args;
size_t NUM_THREADS;
size_t NUM_ITER; 
size_t IMP_METHOD; 
int counter;  
void * (*thread_foo)(void *); 

//Threads
pthread_t* threads;

//Barriers
pthread_barrier_t bar;

//Locks
pthread_mutex_t p_lock; 
TasLock tas_lock;  

struct timespec start, end;

// void global_init(){
// 	threads = (pthread_t *)malloc(NUM_THREADS*sizeof(pthread_t));
// 	args = (size_t*)malloc(NUM_THREADS*sizeof(size_t));
// 	pthread_barrier_init(&bar, NULL, NUM_THREADS);
// 	p_lock = PTHREAD_MUTEX_INITIALIZER;
// 	counter = 0; 
// }

// void global_cleanup(){
// 	free(threads);
// 	free(args);
// 	pthread_barrier_destroy(&bar);
// }

void* thread_main(void* args); 
void* counter_pthread_lock(void* args); 
void* counter_tas_lock(void* args); 
void* counter_tas_lock(void* args); 
void* counter_pthread_bar(void* args); 

void global_init(){

	//Variables used in all cases
	threads = (pthread_t *)malloc(NUM_THREADS*sizeof(pthread_t)); //
	args = (size_t*)malloc(NUM_THREADS*sizeof(size_t));
	pthread_barrier_init(&bar, NULL, NUM_THREADS); //Needed for all the timing analysis
	counter = 0; 

	printf("\nRunning counter with %ld threads, %ld iterations, and method = ", NUM_THREADS, NUM_ITER); 
	
	//Configure global variables based on the input type
	switch (IMP_METHOD){
		case SENSE_BAR:
			/* code */
			printf("SENSE_BAR\n"); 
			thread_foo =  &thread_main; 
			break;
		case PTHREAD_BAR:
			/* code */
			printf("PTHREAD_BAR\n"); 
			thread_foo = &counter_pthread_bar;
			break;
		case TAS_LOCK:
			/* code */
			printf("TAS_LOCK\n"); 
			thread_foo = &counter_tas_lock; 
			break;
		case TTAS_LOCK:
			/* code */
			printf("TTAS_LOCK\n"); 
			thread_foo = &thread_main; 
			break;
		case TICKET_LOCK:
			/* code */
			printf("TICKET_LOCK\n"); 
			thread_foo = &thread_main; 
			break;
		case PTHREAD_LOCK:
			/* code */
			printf("PTHREAD_LOCK\n"); 
			p_lock = PTHREAD_MUTEX_INITIALIZER;
			thread_foo = &counter_pthread_lock; 
			break;

		default:
			break;
		}
}





void global_cleanup(){
	//Variables to free everytime
	free(threads);
	free(args);
	pthread_barrier_destroy(&bar);
	printf("\nCleaning counter with method = "); 
	
	//Variables to free based on configuratoin
	switch (IMP_METHOD){
		case SENSE_BAR:
			/* code */
			printf("SENSE_BAR\n"); 
			 
			break;
		case PTHREAD_BAR:
			/* code */
			printf("PTHREAD_BAR\n"); 
			break;
		case TAS_LOCK:
			/* code */
			printf("TAS_LOCK\n"); 
			// delete &tas_lock; 
		
			break;
		case TTAS_LOCK:
			/* code */
			printf("TTAS_LOCK\n"); 

			break;
		case TICKET_LOCK:
			/* code */
			printf("TICKET_LOCK\n"); 
			
			break;
		case PTHREAD_LOCK:
			/* code */
			printf("PTHREAD_LOCK\n"); 
			break;

		default:
			break;
	}
}





void local_init(){}
void local_cleanup(){}



void* thread_main(void* args){
	size_t tid = *((size_t*)args);
	local_init();
	pthread_barrier_wait(&bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&start);
	}
	pthread_barrier_wait(&bar);
	
	// do something
	printf("Thread %zu reporting for duty\n",tid);
	
	pthread_barrier_wait(&bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&end);
	}
	local_cleanup();
	
	return 0;
}

void* counter_pthread_lock(void* args){
	size_t tid = *((size_t*)args);
	local_init();
	pthread_barrier_wait(&bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&start);
	}
	pthread_barrier_wait(&bar);
	
	// do something
	// printf("counter_pthread_lock: Thread %zu reporting for duty\n",tid);
	for(int i = 0; i < NUM_ITER; i++){
		pthread_mutex_lock(&p_lock); 
		counter++; 
		pthread_mutex_unlock(&p_lock);
	}

	pthread_barrier_wait(&bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&end);
	}
	local_cleanup();
	
	return 0;
}

void* counter_tas_lock(void* args){
	size_t tid = *((size_t*)args);
	local_init();
	pthread_barrier_wait(&bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&start);
	}
	pthread_barrier_wait(&bar);
	
	// do something
	// printf("counter_tas_lock: Thread %zu reporting for duty\n",tid);
	for(int i = 0; i < NUM_ITER; i++){
		// pthread_mutex_lock(&p_lock); 
		tas_lock.lock(); 
		counter++; 
		tas_lock.unlock(); 
		// pthread_mutex_unlock(&p_lock);
	}

	pthread_barrier_wait(&bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&end);
	}
	local_cleanup();
	
	return 0;
}

void* counter_pthread_bar(void* args){
	size_t tid = *((size_t*)args);
	local_init();
	pthread_barrier_wait(&bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&start);
	}
	pthread_barrier_wait(&bar);
	
	//do counting
	for(int i = 0; i < NUM_ITER * NUM_THREADS; i++){
		if(i%NUM_THREADS+1==tid){
			counter++; 
			assert(counter== i+1); 
		}
		pthread_barrier_wait(&bar);
	}


	pthread_barrier_wait(&bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&end);
	}
	local_cleanup();
	
	return 0;
}




int run_threaded_counter(int num_threads, int num_iter, int imp_method, int & final_cntr){
	
	//Update global variables
	NUM_THREADS = num_threads;
	NUM_ITER = num_iter; 
	IMP_METHOD = imp_method; 
	

	// printf("\nRunning counter with %ld threads, %ld iterations, and method = ", NUM_THREADS, NUM_ITER); 
	
	// //Determine which version of counter to run
	// void * (*foo)(void *); 
	
	
	// switch (IMP_METHOD)
	// {
	// case SENSE_BAR:
	// 	/* code */
	// 	printf("SENSE_BAR\n"); 
	// 	foo =  &thread_main; 
	// 	break;
	// case PTHREAD_BAR:
	// 	/* code */
	// 	printf("PTHREAD_BAR\n"); 
	// 	foo = &counter_pthread_bar;
	// 	break;
	// case TAS_LOCK:
	// 	/* code */
	// 	printf("TAS_LOCK\n"); 
	// 	foo = &counter_tas_lock; 
	// 	break;
	// case TTAS_LOCK:
	// 	/* code */
	// 	printf("TTAS_LOCK\n"); 
	// 	foo = &thread_main; 
	// 	break;
	// case TICKET_LOCK:
	// 	/* code */
	// 	printf("TICKET_LOCK\n"); 
	// 	foo = &thread_main; 
	// 	break;
	// case PTHREAD_LOCK:
	// 	/* code */
	// 	printf("PTHREAD_LOCK\n"); 
	// 	foo = &counter_pthread_lock; 
	// 	break;

	// default:
	// 	break;
	// }

	global_init();
	

	// launch threads
	int ret; size_t i;
  	for(i=1; i<NUM_THREADS; i++){
		args[i]=i+1;
		printf("creating thread %zu\n",args[i]);
		ret = pthread_create(&threads[i], NULL, thread_foo, &args[i]);
		if(ret){
			printf("ERROR; pthread_create: %d\n", ret);
			exit(-1);
		}
	}
	i = 1;

	(*thread_foo)(&i); // master also calls thread_main
	// join threads
	for(size_t i=1; i<NUM_THREADS; i++){
		ret = pthread_join(threads[i],NULL);
		if(ret){
			printf("ERROR; pthread_join: %d\n", ret);
			exit(-1);
		}
		printf("joined thread %zu\n",i+1);
	}
	printf("\ncounter = %d\n",counter); 
	
	//Check that counter is the right value
	assert(counter == NUM_THREADS *NUM_ITER); 
	
	//Return the counter
	final_cntr = counter; 
	
	//Clean up
	global_cleanup();
	
	unsigned long long elapsed_ns;
	elapsed_ns = (end.tv_sec-start.tv_sec)*1000000000 + (end.tv_nsec-start.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
	return 0; 
}