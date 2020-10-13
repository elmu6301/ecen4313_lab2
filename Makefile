# ECEN 4313: Concurrent Programming
# Author: Elena Murray
# Date: X/XX/2020
# Lab X: 
# 	Makefile

#Executable
main: main.o
	g++ main.o -o main

#Object Files
main.o: main.cpp
	g++ -c main.cpp

#Clean
clean:
	rm*.o main