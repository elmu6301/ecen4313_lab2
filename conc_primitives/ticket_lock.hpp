#ifndef TICKET_LOCK_HPP
#define TICKET_LOCK_HPP
/*
ECEN 4313: Concurrent Programming
Author: Elena Murray
Date: 10/17/2020
Lab 2: 
    
*/
#include <stdio.h>
#include <atomic>

class TicketLock{
    private: 
    std::atomic<int> next_num;  
    std::atomic<int> now_serving; 
    
    public: 
    TicketLock(); 
    void lock(); 
    void unlock();  

}; 
 void test_ticket_lock(); 

#endif