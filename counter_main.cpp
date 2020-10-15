/*
ECEN 4313: Concurrent Programming
Author: Elena Murray
Date: 8/30/2020
Lab 2: 

*/

//Library includes
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

//Global Variables
char my_name[] = "Elena Murray"; 


/*
    Prints the expected commandline call. 
*/
void printUsage(){
    printf("counter [--name] [-t NUM_THREADS] [-i=NUM_ITERATIONS] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,mcs,pthread>] [-o outfile.txt]\n");
}


//main function
int main(int argc, char* argv[]){
    // cout <<"Hello world " <<my_name<<endl ; 
    // printUsage(); 

    //variables for parsing the command line
    string outFile; //stores the name of the file to output the data
    string threads; 
    string iterations;  
    string bar; 
    string lock;
    char opt; //stores the option value
    int num_threads = 1; //stores the number of threads to create, set to default of 1 thread (i.e. master thread)
    int num_iter = 1; //stores the number of iterations to run default 1. 

    static struct option longopt[] = {
        {"name", no_argument, NULL, 'n'},// --name
        {"o", required_argument, NULL, 'o'}, // output file
        {"t", required_argument, NULL, 't'}, // threads
        {"i", required_argument, NULL, 'i'}, // iterations
        {"bar", required_argument, NULL, 'b'}, // barrier implementation
        {"lock", required_argument, NULL, 'l'} // barrier implementation
    }; 
    char * optstr = "no:t:i:b:l:"; 


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
            case 'i':
                iterations = optarg; 
                break; 
            case 'b':
                bar = optarg; 
                break; 
            case 'l':
                lock = optarg; 
                break; 
        }
    }
    cout<<"Input arguments:"<<endl; 
    cout<<"threads = "<<threads<<endl; 
    cout<<"iterations = "<<iterations<<endl; 
    cout<<"bar = "<<bar<<endl; 
    cout<<"lock = "<<lock<<endl; 
    cout<<"outFile = "<<outFile<<endl; 

    //Check options to make sure that they are valid
    if(outFile.rfind(".txt")==string::npos){
        cout<<"The output file: "<<outFile<<" is not a .txt file."<<endl; 
        printUsage(); 
        return -2;
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

    //Convert iterations to an integer
    if(iterations.empty()){
        cout<<"No iterations were entered. Running the program with the default number of iterations. "<<endl; 
    }else{
        //Check to see if the first char is an = and remove if needed
        if(iterations[0] == '='){
            iterations.assign(iterations,1,iterations.length()); 
            cout<<"iterations = "<<iterations<<endl; 
        }
        try{
            num_iter = stoi(iterations); 
        }catch( exception &e){
            //Print message
            cout<<"An invalid iteration count was entered. Try using an integer value."<<endl; 
            printUsage(); 
            return -4; 
        }
        //Not enough threads
        if(num_iter<1){
            cout<<"An invalid iterationcount was entered. At least one iteration is required.\nThe number of iterations to run must be greater than 1."<<endl; 
            printUsage(); 
            return -4; 
        }
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
    }

    cout<<"Running counter with "<<num_threads<<" threads, "<<num_iter<<" iterations, '"<<bar<<"' barriers, '"<<lock<<"' locks, and outputting to '"<<outFile<<"'"<<endl; 
    
    int final_cnt = 100; 
    //Call counter here


    // Output sorted data to output file
    ofstream fileOut; 
    fileOut.open(outFile); 
     if(!fileOut){
        cout<<"Unable to open the file: "<<outFile<<endl; 
        return -4; 
    }

    fileOut<<final_cnt<<endl; 

    fileOut.close();  





}