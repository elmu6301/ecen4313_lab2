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
#include <unistd.h>
#include <atomic>

using namespace std;

//Developer includes
#include "tas_lock.hpp"

/*************************************************
	FUNCTIONS
**************************************************/
void test_tas_lock(){
    printf("\ntas_lock\n"); 
}

/*************************************************
	CLASS FUNCTIONS
**************************************************/
/*
    Constructor
*/
TasLock::TasLock(){
   flag.store(false); 
}

/*
    Aquires the lock
*/
void TasLock::lock(){
    //Since flag is an atomic bool using exchange for tas implemenation
    while(flag.exchange(true)==true); 
}

/*
    Releases the lock
*/
void TasLock::unlock(){
    flag.store(false); 
}
