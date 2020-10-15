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


int run_threaded_bucketsort(int num_threads,std::vector <int> &data ); 

#endif