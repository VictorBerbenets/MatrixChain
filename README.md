# Matrix chain
## About
This is the second level of the [matrix task](https://github.com/VictorBerbenets/Matrix).
The main goal is to implement a multifunctional class for working with matrices, namely, to implement effective multiplication of a chain of matrices.  
***Main algorithm***  
In this program, the main role is played by the dynamic programming algorithm, which can be found in [Cormen](https://e-maxx.ru/bookz/files/cormen.pdf) .  
In short, a main method `get_optimal_mul_order()` consists of 4 steps:  
1. Find the optimal arrangement of brackets for multiplying a chain of matrices. The solution to this problem is made up of optimal solutions to subtasks (we divide the chain into sub-chains and look for the best ways to multiply them).
2. Recursively determine the cost of the optimal solution in terms
of optimal solutions to subtasks.
3. We calculate the minimum cost of the product of a chain of matrices using a tabular ascending approach (in one table we store the costs of calculating the sub-chains, and in the other the indices at which optimal costs are achieved).
4. Using the `optimal_order()` method and a table with optimal values, we recursively restore the optimal multiplication order of the matrix chain.  

***Class interface***  
The MatrixChain class contains 2 fields of type `std::deque<Matrix<numeric_type>>` and `std::deque<std::size_t>`. The first stores the matrices themselves, and the second stores their sizes in the appropriate order. Note that the `std::deque` container is the best suited for this task: we have insertion, deletion at the end and beginning for 0(1) and indexing for O(1).  
It is also possible to efficiently and directly multiply a chain of matrices. The efficient method uses the optimal multiplication order returned by the `get_optimal_order()` function, then in the loop, as multiplication progresses, `std::map<std::size_t, matrix_type>` is filled in. As a result, the product of the most recently multiplied pair of matrices is returned.
## Requirements
**cmake** version must be 3.15 or higher
## How to build
```bash
git clone --recurse-submodules git@github.com:VictorBerbenets/MatrixChain.git
cd MatrixChain/
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
***Make sure you have compiled with `-DEND2END_TESTING=ON`***  
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

