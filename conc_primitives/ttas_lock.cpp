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
    flag.store(false); 
}

void TtasLock::myTtAS(){
    bool res = flag.load(); 
    printf(res ? "true": "false"); 
}

void TtasLock::lock(){
    while(flag.load() == true || flag.exchange(true) == true); 

}

void TtasLock::unlock(){
    flag.store(false); 
}
