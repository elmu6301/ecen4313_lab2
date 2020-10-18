#ifndef ARRAY_SPLITTER_HPP
#define ARRAY_SPLITTER_HPP
/*
ECEN 4313: Concurrent Programming
Author: Elena Murray
Date: 9/21/2020
Lab 1: 
    
*/

#include <vector>

std::vector<std::vector<int>> split_vector_array(std::vector<int>array,int num_parts); 
float calc_stand_dev(std::vector<int> data); 
int calc_num_buckets(std::vector<int> data, int num_threads); 
int calc_max(std::vector<int> data); 

#endif