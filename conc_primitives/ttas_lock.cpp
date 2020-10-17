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
#include "ttas_lock.hpp"

//Functions
void test_ttas_lock(){
    printf("\nttas_lock\n"); 
}

//Class functions

/*
    Constructor
*/
TtasLock::TtasLock(){
   flag.clear(); 
}

void TtasLock::myTtAS(){
    bool res = flag.test_and_set(); 
    printf(res ? "true": "false"); 
}

void TtasLock::lock(){
    // while(flag.atomic_load() ||flag.test_and_set() == false); 
}

void TtasLock::unlock(){
    // flag.clear(); 
}
