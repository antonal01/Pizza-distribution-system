# PizzaOrdering-distribution-system
 A simulation of a pizza ordering distribution system, with the use of the POSIX threads (pthread libraries) in C.
 
The system implements mutual exclusion (with mutexes) and synchronization (with condition variables) to manage large
numbers of orders with a limited number of service points and distributors. 
 
Each order corresponds to one thread.


## Usage
 The program accepts two parameters, the first parameter is the number of customers to
 service, and the second the seed for reproducing random numbers.

```
gcc -Wall -pthread -o testPizza pizza.c pizza.h
./testPizza 100 1000
```
