# Pizza-distribution-system
 A pizza ordering distribution system with use of the POSIX threads (pthreads) package in C.
 
The program accepts two parameters, the first parameter is the number of customers to
service, and the second the seed for reproducing random numbers.

Each order corresponds to one thread.

In these systems we have a large number of orders which are served by one
limited number of service points and distributors therefore the program implements
mutual exclusion (with mutexes) and synchronization (with condition variables).
