ECEN 4313 Concurrent Programming
LAB 2 Write Up
Elena Murray

1. Overview
2. File Description
3. Compliation Instruction
4. Execution Instruction
5. Known Bugs

./counter -t 3 -i 2 --lock=pthread -o output.txt
./counter -t 3 -i 2 --lock=tas -o output.txt
./counter -t 3 -i 2 --bar=pthread -o output.txt