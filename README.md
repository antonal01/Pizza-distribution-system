# Pizza ordering-distribution system
 A simulation of a pizza ordering distribution system, with the use of the POSIX threads (pthread libraries) in C.
 
The system implements mutual exclusion (with mutexes) and synchronization (with condition variables) to manage large
numbers of orders with a limited number of service points and distributors. 

## Running process

Each order corresponds to one thread.
The cooks, ovens and the distributors mentioned below, are represented as numeric counters.

Each thread checks the available number of cooks and in case of availability, uses him.
Otherwise, in case of unavailability the thread waits and repeats the procedure when it is signaled.

Afterwards, the thread checks the available number of ovens and in case of availability, releases the cook and uses an oven.
Like before, in case of unavailability the thread waits and repeats the procedure when it is signaled.

Similarly, the thread checks the available number of distributors and in case of availability, releases the oven and uses a distributor.
Once again, in case of unavailability the thread waits and repeats the procedure when it is signaled.

Finally, when the distribution is over the thread releases a distributor.
###### Note
When we say

## Usage
 The program accepts two parameters, the first parameter is the number of customers to
 service, and the second the seed for reproducing random numbers.

```
gcc -Wall -pthread -o testPizza pizza.c pizza.h
./testPizza 100 1000
```
