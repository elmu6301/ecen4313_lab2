#ifndef TAS_LOCK_HPP
#define TAS_LOCK_HPP
/*
ECEN 4313: Concurrent Programming
Author: Elena Murray
Date: 10/17/2020
Lab 2: 
    
*/
#include <stdio.h>
#include <atomic>

class TasLock{
    private: 
    std::atomic<bool> flag;

    public: 
    TasLock(); 
    void lock(); 
    void unlock();  

}; 
 void test_tas_lock(); 

#endif