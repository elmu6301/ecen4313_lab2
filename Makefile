# ECEN 4313: Concurrent Programming
# Author: Elena Murray
# Date: 10/14/2020
# Lab 2: 
# 	Makefile

#Make all executables
all: mysort counter 

#Executable for mysort
mysort: bucketsort_main.o threaded_bucketsort.o array_splitter.o
	g++ bucketsort_main.o threaded_bucketsort.o array_splitter.o -pthread -g -o mysort

#Object Files for mysort
bucketsort_main.o: bucketsort_main.cpp
	g++ -c bucketsort_main.cpp

array_splitter.o: array_splitter.cpp array_splitter.hpp
	g++ -c array_splitter.cpp

threaded_bucketsort.o: threaded_bucketsort.cpp threaded_bucketsort.hpp
	g++ -c threaded_bucketsort.cpp 

#Executable for counter
counter: counter_main.o
	g++ counter_main.o -o counter

#Object Files for counter
counter_main.o: counter_main.cpp
	g++ -c counter_main.cpp


#Clean
clean:
	rm *.o mysort counter
