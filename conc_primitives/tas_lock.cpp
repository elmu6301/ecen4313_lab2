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
#include "tas_lock.hpp"

//Functions
void test_tas_lock(){
    printf("\ntas_lock\n"); 
}

//Class functions

/*
    Constructor
*/
TasLock::TasLock(){
//    flag.clear(); 
   flag.store(false); 
}

void TasLock::myTAS(){
    // bool res = flag.test_and_set(); 
    bool res = flag.load(); 
    printf(res ? "true": "false"); 
}

void TasLock::lock(){
    // while(flag.test_and_set() == false); 
    while(flag.exchange(true)==true); 
}

void TasLock::unlock(){
    // flag.clear(); 
    flag.store(false); 
}
