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
   flag.clear(); 
}

void TasLock::myTAS(){
    bool res = flag.test_and_set(); 
    printf(res ? "true": "false"); 
}

void TasLock::lock(){
    while(flag.test_and_set() == false); 
}

void TasLock::unlock(){
    flag.clear(); 
}
