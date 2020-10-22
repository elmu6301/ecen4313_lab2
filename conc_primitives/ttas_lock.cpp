/*
ECEN 4313: Concurrent Programming
Author: Elena Murray
Date: 9/30/2020
Lab 2: 
    
*/
/*************************************************
	FILE INCLUDES
**************************************************/
//Library includes
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <atomic>

using namespace std; 

//Developer includes
#include "ttas_lock.hpp"

/*************************************************
	FUNCTIONS
**************************************************/
void test_ttas_lock(){
    printf("\nttas_lock\n"); 
}

/*************************************************
	CLASS FUNCTIONS
**************************************************/
/*
    Constructor
*/
TtasLock::TtasLock(){
    flag.store(false); 
}

/*
    Aquires the lock
*/
void TtasLock::lock(){
    while(flag.load() == true || flag.exchange(true) == true); 

}

/*
    Releases the lock
*/
void TtasLock::unlock(){
    flag.store(false); 
}
