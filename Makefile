# ECEN 4313: Concurrent Programming
# Author: Elena Murray
# Date: 10/14/2020
# Lab 2: 
# 	Makefile

#Make all executables
all: mysort counter 

#Executable for mysort
mysort: bucketsort_main.o threaded_bucketsort.o array_splitter.o sense_barrier.o tas_lock.o ttas_lock.o ticket_lock.o
	g++ bucketsort_main.o threaded_bucketsort.o array_splitter.o sense_barrier.o tas_lock.o ttas_lock.o ticket_lock.o -pthread -g -o mysort

#Object Files for mysort
bucketsort_main.o: mysort_files/bucketsort_main.cpp
	g++ -c mysort_files/bucketsort_main.cpp

array_splitter.o: mysort_files/array_splitter.cpp mysort_files/array_splitter.hpp
	g++ -c mysort_files/array_splitter.cpp

threaded_bucketsort.o: mysort_files/threaded_bucketsort.cpp mysort_files/threaded_bucketsort.hpp
	g++ -c mysort_files/threaded_bucketsort.cpp 

#Executable for counter
counter: counter_main.o threaded_counter.o tas_lock.o ttas_lock.o ticket_lock.o sense_barrier.o
	g++ counter_main.o  threaded_counter.o tas_lock.o ttas_lock.o ticket_lock.o sense_barrier.o -pthread -g -o counter

#Object Files for counter
counter_main.o: counter_files/counter_main.cpp
	g++ -c counter_files/counter_main.cpp

threaded_counter.o: counter_files/threaded_counter.cpp counter_files/threaded_counter.hpp
	g++ -c counter_files/threaded_counter.cpp 


#Object Files for Locks
tas_lock.o: conc_primitives/tas_lock.cpp conc_primitives/tas_lock.hpp
	g++ -c conc_primitives/tas_lock.cpp

ttas_lock.o: conc_primitives/ttas_lock.cpp conc_primitives/ttas_lock.hpp
	g++ -c conc_primitives/ttas_lock.cpp

ticket_lock.o: conc_primitives/ticket_lock.cpp conc_primitives/ticket_lock.hpp
	g++ -c conc_primitives/ticket_lock.cpp

sense_barrier.o: conc_primitives/sense_barrier.cpp conc_primitives/sense_barrier.hpp
	g++ -c conc_primitives/sense_barrier.cpp


#Clean
clean:
	rm *.o mysort counter
