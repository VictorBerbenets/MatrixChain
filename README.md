# Matrix chain
## About
This is the second level of the [matrix task](https://github.com/VictorBerbenets/Matrix).
The main goal is to implement a multifunctional class for working with matrices, namely, to implement effective multiplication of a chain of matrices.
## Requirements
**cmake** version must be 3.15 or higher
## How to build
```bash
git clone git@github.com:VictorBerbenets/MatrixChain.git
cd MatrixChain/
git submodule init
git submodule update
cmake -S ./ -B build/ -DCMAKE_BUILD_TYPE=Release .. [-DEND2END_TESTING=<OFF/ON>] [-DEFFECTIVE_MULTIPLY=<OFF/ON>]
cd build/
cmake --build .
```
## To Run the program do  
```bash
./matrix_chain
```
The program will be waiting for input data in stdin in such way:  
```bash
<matrices sizes> <size1> <size2> ...  
```
At the end, the program prints optimal multiplication order for the chain of
matrices. Example:  
input:  
5  
30 35 15 5 10  
output:  
1 0 2
***Make sure you have compiled with `-DEND2END_TESTING=OFF`.***  
## How to run tests:
### You can run unit tests:
```bash
./tests/unit_chain
```
### Or you can run end2end tests:
*Make sure* you have compiled with:  
```bash
cmake -S ./ -B build/ -DCMAKE_BUILD_TYPE=Release .. -DEND2END_TESTING=ON
```
In that case do:  
```bash
bash ../tests/end2end/test_runner.sh <tests_number>
```
**tests_number** - the number of tests you want to generate.  
After running you can see a generated directory - tests/end2end/resources/.  
There will be the tests themselves and the answers to them.  

## How to compare chain multiplications
```bash
./tests/performance
```
the multiplication method (effective or direct) will be chosen depending on the  
initial build of the project(**-DEFFECTIVE_MULTIPLY=<OFF/ON>**). If you want to change  
the multiplication method, do the following:  
```bash
cd MatrixChain/
cmake -S ./ -B build/ -DCMAKE_BUILD_TYPE=Release .. -DEFFECTIVE_MULTIPLY=<OFF/ON>
cmake --build ..
```

