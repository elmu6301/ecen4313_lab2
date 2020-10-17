#ifndef THREADED_COUNTER_HPP
#define THREADED_COUNTER_HPP
/*
ECEN 4313: Concurrent Programming
Author: Elena Murray
Date: 10/16/2020
Lab 2: 
    
*/
#include <vector>
#include <list>
#include <stdio.h>


//Enumerations to detetermine the type of primitives to use
enum counter_imp{
    SENSE_BAR, 
    PTHREAD_BAR,
    TAS_LOCK,
    TTAS_LOCK,
    TICKET_LOCK,
    PTHREAD_LOCK
}; 


int run_threaded_counter(int num_threads, int num_iter, int imp_method, int & final_cntr); 

#endif