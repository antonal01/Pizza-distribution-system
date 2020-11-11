# Pizza-distribution-system
 A simulation of a pizza ordering distribution system, with the use of the POSIX threads (pthread libraries) in C.
 
 Orders are served by one limited number of service points and distributors, therefore the program implements
 mutual exclusion (with mutexes) and synchronization (with condition variables).
 
 Each order corresponds to one thread.
 
 The program accepts two parameters, the first parameter is the number of customers to
 service, and the second the seed for reproducing random numbers.

## Usage

```
gcc -Wall -pthread -o testPizza pizza.c pizza.h
./testPizza 100 1000
```
