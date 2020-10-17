/*
ECEN 4313: Concurrent Programming
Author: Elena Murray
Date: 9/30/2020
Lab 2: 
    
*/

//Library includes
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <atomic>

using namespace std; 
//Developer includes
#include "ticket_lock.hpp"

//Functions
void test_ticket_lock(){
    printf("\nticket_lock\n"); 
}

//Class functions

/*
    Constructor
*/
TicketLock::TicketLock(){
    next_num.store(0); 
    now_serving.store(0); 
}


void TicketLock::lock(){
    int my_num = atomic_fetch_add_explicit(&next_num, 1, memory_order_seq_cst); 
    while(atomic_load_explicit(&now_serving,memory_order_seq_cst) !=my_num); 
}

void TicketLock::unlock(){
    atomic_fetch_add_explicit(&now_serving, 1, memory_order_seq_cst); 
}
