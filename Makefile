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
counter: counter_main.o threaded_counter.o tas_lock.o ttas_lock.o ticket_lock.o
	g++ counter_main.o  threaded_counter.o tas_lock.o ttas_lock.o ticket_lock.o -pthread -g -o counter

#Object Files for counter
counter_main.o: counter_main.cpp
	g++ -c counter_main.cpp

threaded_counter.o: threaded_counter.cpp threaded_counter.hpp
	g++ -c threaded_counter.cpp 


#Object Files for Locks
tas_lock.o: conc_primitives/tas_lock.cpp conc_primitives/tas_lock.hpp
	g++ -c conc_primitives/tas_lock.cpp

ttas_lock.o: conc_primitives/ttas_lock.cpp conc_primitives/ttas_lock.hpp
	g++ -c conc_primitives/ttas_lock.cpp

ticket_lock.o: conc_primitives/ticket_lock.cpp conc_primitives/ticket_lock.hpp
	g++ -c conc_primitives/ticket_lock.cpp

#Clean
clean:
	rm *.o mysort counter
