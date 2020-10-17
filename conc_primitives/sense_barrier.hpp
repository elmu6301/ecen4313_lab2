#ifndef SENSE_BARRIER_HPP
#define SENSE_BARRIER_HPP
/*
ECEN 4313: Concurrent Programming
Author: Elena Murray
Date: 10/17/2020
Lab 2: 
    
*/
#include <stdio.h>
#include <atomic>

class SenseBarrier{
    private: 
    std::atomic<int> cnt;  
    std::atomic<bool> sense;
    int N; 
    
    public: 
    void init(int NUM_THREADS); 
    void wait(); 

}; 
 void test_sense_bar(); 

#endif