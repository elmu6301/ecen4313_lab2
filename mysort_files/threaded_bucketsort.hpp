#ifndef THREADED_BUCKETSORT_HPP
#define THREADED_BUCKETSORT_HPP
/*
ECEN 4313: Concurrent Programming
Author: Elena Murray
Date: 9/21/2020
Lab 1: 
    
*/
#include <vector>
#include <list>

struct threaded_bucketsort_args {
    size_t tid; //Thread id
    int max; //Max value
    std::vector<int> array; 
    std::list<int> * buckets; 
}; 

//Enumerations to detetermine the type of primitives to use
enum mysort_imp{
    //Lock with sense barrier for timing
    TAS_LOCK_SENSE_BAR,
    TTAS_LOCK_SENSE_BAR,
    TICKET_LOCK_SENSE_BAR,
    PTHREAD_LOCK_SENSE_BAR,
    SENSE_BAR_SENSE_BAR,
    //Lock with pthread barrier for timing
    TAS_LOCK_PTHREAD_BAR, 
    TTAS_LOCK_PTHREAD_BAR, 
    TICKET_LOCK_PTHREAD_BAR, 
    PTHREAD_LOCK_PTHREAD_BAR, 
    SENSE_BAR_PTHREAD_BAR
}; 

int run_threaded_bucketsort(int num_threads, int imp_method, std::vector <int> &data ); 

#endif