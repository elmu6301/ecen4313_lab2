/*
ECEN 4313: Concurrent Programming
Author: Elena Murray
Date: 8/30/2020
Lab 2: 

*/

//Library includes
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <getopt.h>
#include <vector>
#include <string>
#include <exception> 

using namespace std; 

//Developer includes
#include "threaded_bucketsort.hpp"


//Global Variables
string my_name = "Elena Murray"; //full name 



/*
    Prints the expected commandline call. 
*/
void printUsage(){
    printf("mysort [--name] [sourcefile.txt] [-o outfile.txt] [-t NUM_THREADS][--alg=<fj,bucket>] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,mcs,pthread>]\n");
}

//main function
int main(int argc, char* argv[]){ 

    //variable for parsing the command line
    string srcFile; //stores the name of the file to read from 
    string outFile; //stores the name of the file to output the data
    string threads; 
    string alg; //stores the algorithm to be used either q (quicksort) or m (mergesort) 
    string bar; 
    string lock; 
    char opt; //stores the option value
    int num_threads = 1; //stores the number of threads to create, set to default of 1 thread (i.e. master thread)
    int imp_lock_method = PTHREAD_LOCK; 
    //  int imp_lock_method = PTHREAD_LOCK; 
   
    static struct option longopt[] = {
        {"name", no_argument, NULL, 'n'},// --name
        {"o", required_argument, NULL, 'o'}, // output file
        {"t", required_argument, NULL, 't'}, // threads
        {"alg", required_argument, NULL, 'a'}, // algorithm
        {"bar", required_argument, NULL, 'b'}, // barrier implementation
        {"lock", required_argument, NULL, 'l'} // barrier implementation

    }; 
    char * optstr = "no:t:a:b:l:"; 
    //Get the source file
    if(argc > 1){
        srcFile = argv[1];  
    }
    //Parse the rest of the command line
    while((opt = getopt_long(argc, argv, optstr, longopt, NULL))!=-1){
        // cout<<opt<<endl; 
        switch(opt){
            case 'n': 
                cout<<my_name<<endl; 
                return 1; 
                break; 
            case 'o':
                outFile = optarg; 
                break; 
            case 't':
                threads = optarg; 
                break; 
            case 'a':
                alg = optarg; 
                break; 
            case 'b':
                bar = optarg; 
                break; 
            case 'l':
                lock = optarg; 
                break; 
        }
    }
   
    //Check options to make sure that they are valid
    if(srcFile.rfind(".txt")==string::npos){
        cout<<"The source file: "<<srcFile<<" is not a .txt file."<<endl; 
        printUsage(); 
        return -1;
    }

    if(outFile.rfind(".txt")==string::npos){
        cout<<"The output file: "<<outFile<<" is not a .txt file."<<endl; 
        printUsage(); 
        return -2;
    }   

    if(alg.compare("fj")!= 0 && alg.compare("bucket")!= 0){
        cout<<"An invalid algorithm was entered."<<endl; 
        printUsage();
        return -3; 
    }

    if(bar.compare("pthread")!= 0 && bar.compare("sense")!= 0){
        cout<<"An invalid barrier implementation was entered."<<endl; 
        printUsage();
        return -4; 
    }

    if(lock.compare("pthread")!= 0 && lock.compare("tas")!= 0 && lock.compare("ttas")!= 0 && lock.compare("ticket")!= 0){
        cout<<"An invalid lock implementation was entered."<<endl; 
        printUsage();
        return -5; 
    }else if(lock.compare("pthread")==0){
        imp_lock_method = PTHREAD_LOCK; 
    }else if(lock.compare("tas")==0){
        imp_lock_method = TAS_LOCK; 
    }else if(lock.compare("ttas")==0){
        imp_lock_method = TTAS_LOCK; 
    }else if(lock.compare("ticket")==0){
        imp_lock_method = TICKET_LOCK; 
    }
  
    //Convert threads to an integer
    if(threads.empty()){
        cout<<"No threads were entered. Running the program with only the master thread (NUM_THREAD = 1). "<<endl; 
    }else{
        try{
            num_threads = stoi(threads); 
        }catch( exception &e){
            //Print message
            cout<<"An invalid thread count was entered. Try using an integer value."<<endl; 
            printUsage(); 
            return -4; 
        }
        //Not enough threads
        if(num_threads<1){
            cout<<"An invalid thread count was entered. A master thread is required.\nThe number of threads to run must be greater than 1."<<endl; 
            printUsage(); 
            return -4; 
        }
    }
    

    
    //Open sourcefile
    ifstream fileIn;  
    fileIn.open(srcFile); 
    if(!fileIn){
        cout<<"Unable to open the file: "<<srcFile<<endl; 
        return -4; 
    }

    //Read in the input
    vector <int> data; 
    string line; 
    int el; 
    while(getline(fileIn, line)){
        try{
            el = stoi(line); 
            data.push_back(el); 
        }catch(exception & e){
            cout<<"Error: invalid data detected."<<endl; 
            fileIn.close(); 
            return -4; 
        }
    }
    fileIn.close(); 
    int data_size = data.size(); 
    //Check to make sure that the size of data is larger than the number of threads
    if(num_threads>data_size){
        cout<<"An invalid thread count was entered. The number of threads cannot exceed the amount of data."<<endl; 
        printUsage(); 
        return -4; 
    }

    cout<<"Running counter with "<<num_threads<<" threads, '"<<bar<<"' barriers, '"<<lock<<"' locks, input file '"<<srcFile<<"', and outputting to '"<<outFile<<"'"<<endl; 
   
    if(bar.compare("pthread")==0){
        //mergeSort(data);  
        cout<<"Running bucket sort with pthread barriers"<<endl; 
        run_threaded_bucketsort(num_threads, imp_lock_method, data); 
    }
    // else if(bar.compare("sense")==0){
    //      cout<<"Running bucket sort with sense barriers"<<endl; 
    //     run_threaded_bucketsort_s_bar(num_threads, data); 
    // }

    // Output sorted data to output file
    ofstream fileOut; 
    fileOut.open(outFile); 
     if(!fileOut){
        cout<<"Unable to open the file: "<<outFile<<endl; 
        return -4; 
    }

    for(int i = 0; i < data.size(); i++){
        fileOut<<data[i]<<endl; 
    }

    fileOut.close();    
}


