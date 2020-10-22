/*
ECEN 4313: Concurrent Programming
Author: Elena Murray
Date: 9/30/2020
Lab 2: 
    
*/

/*************************************************
	FILE INCLUDES
**************************************************/
//Library includes
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <atomic>

using namespace std;

//Developer includes
#include "sense_barrier.hpp"

/*************************************************
	FUNCTIONS
**************************************************/
void test_sense_bar(){
    printf("\nsense_barrier\n"); 
}

/*************************************************
	CLASS FUNCTIONS
**************************************************/
/*
    Constructor
*/
void SenseBarrier::init(int NUM_THREADS){
    cnt.store(0); 
    sense.store(false); 
    N = NUM_THREADS; 
}


/*
    Barrier wait.
*/
void SenseBarrier::wait(){
    thread_local bool my_sense = false; 
    //Flip sense
    my_sense = !my_sense; 

    //Get the current thread count
    int cnt_cpy = atomic_fetch_add_explicit(&cnt, 1, memory_order_seq_cst); 
    
    //Check to see if we are the last thread
    if(cnt_cpy == N-1){
        //reset the counter
        atomic_store_explicit(&cnt, 0, memory_order_relaxed); 
        //flip the overall sense
        atomic_store_explicit(&sense, my_sense, memory_order_seq_cst); 
    }else{// otherwise wait till the last thread arrives
        while(atomic_load_explicit(&sense, memory_order_seq_cst)!=my_sense); 
    }
}