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
    SENSE_BAR, 
    PTHREAD_BAR,
    TAS_LOCK,
    TTAS_LOCK,
    TICKET_LOCK,
    PTHREAD_LOCK

}; 

int run_threaded_bucketsort(int num_threads, int imp_method, std::vector <int> &data ); 
int run_threaded_bucketsort_s_bar(int num_threads,std::vector <int> &data ); 

#endif