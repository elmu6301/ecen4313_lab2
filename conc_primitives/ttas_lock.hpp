#ifndef TTAS_LOCK_HPP
#define TTAS_LOCK_HPP
/*
ECEN 4313: Concurrent Programming
Author: Elena Murray
Date: 10/17/2020
Lab 2: 
    
*/
#include <stdio.h>
#include <atomic>

class TtasLock{
    private: 
    std::atomic_flag flag = ATOMIC_FLAG_INIT; 
    
    public: 
    TtasLock(); 
    void myTtAS();
    void lock(); 
    void unlock();  

}; 
 void test_ttas_lock(); 

#endif