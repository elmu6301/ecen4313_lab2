#!/bin/bash

#Test against 
iter=20
threads=10
while getopts t:i: flag
do 
    case "${flag}" in
        t) threads=${OPTARG};; 
        i) iter=${OPTARG};; 
    esac
done

#Files
test_out="test_files/counter_output.txt"
test_sol="test_files/counter_soln.txt"
test_res=$((iter * threads))
echo $test_res > ${test_sol}

echo "Running counter with:"
echo "Threads: $threads"
echo "Iterations: $iter"
echo "Output File: $test_out"
echo "Expected Output: $test_sol"

test_res=('SUCCESS' 'SUCCESS' 'SUCCESS' 'SUCCESS' 'SUCCESS' 'SUCCESS')
i=0
n=6
echo 

#Barrier Tests
echo "Test Case ${i}-> -t ${threads} --bar=pthread"
rm ${test_out}
perf stat -e L1-dcache-load-misses -e L1-dcache-load -e branch-misses -e branches ./counter -o ${test_out}  -t ${threads} -i ${iter} --bar=pthread 
cmp ${test_out} ${test_sol} && test_res[$i]="SUCCESS" || test_res[$i]="FAILED"
i=$((i + 1))
echo 

echo "Test Case ${i}-> -t ${threads} --bar=sense"
rm ${test_out}
perf stat -e L1-dcache-load-misses -e L1-dcache-load -e branch-misses -e branches ./counter -o ${test_out}  -t ${threads} -i ${iter} --bar=sense 
cmp ${test_out} ${test_sol} && test_res[$i]="SUCCESS" || test_res[$i]="FAILED"
i=$((i + 1))
echo 

#Lock Tests
echo "Test Case ${i}-> -t ${threads} --lock=pthread"
rm ${test_out}
perf stat -e L1-dcache-load-misses -e L1-dcache-load -e branch-misses -e branches ./counter -o ${test_out}  -t ${threads} -i ${iter} --lock=pthread 
cmp ${test_out} ${test_sol} && test_res[$i]="SUCCESS" || test_res[$i]="FAILED"
i=$((i + 1))
echo 

echo "Test Case ${i}-> -t ${threads} --lock=tas"
rm ${test_out}
perf stat -e L1-dcache-load-misses -e L1-dcache-load -e branch-misses -e branches ./counter -o ${test_out}  -t ${threads} -i ${iter} --lock=tas
cmp ${test_out} ${test_sol} && test_res[$i]="SUCCESS" || test_res[$i]="FAILED"
i=$((i + 1))
echo 

echo "Test Case ${i}-> -t ${threads} --lock=ttas"
rm ${test_out}
perf stat -e L1-dcache-load-misses -e L1-dcache-load -e branch-misses -e branches ./counter -o ${test_out}  -t ${threads} -i ${iter} --lock=ttas
cmp ${test_out} ${test_sol} && test_res[$i]="SUCCESS" || test_res[$i]="FAILED"
i=$((i + 1))
echo 

echo "Test Case ${i}-> -t ${threads} --lock=ticket"
rm ${test_out}
perf stat -e L1-dcache-load-misses -e L1-dcache-load -e branch-misses -e branches ./counter -o ${test_out}  -t ${threads} -i ${iter} --lock=ticket
cmp ${test_out} ${test_sol} && test_res[$i]="SUCCESS" || test_res[$i]="FAILED"
i=$((i + 1))
echo 

echo "Results:"
i=0
while [ $i -lt $n ]
do
    echo "TEST[${i}]--> ${test_res[$i]}"
    i=$((i + 1))
done