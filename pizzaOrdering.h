#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

unsigned int globalSeed;
int tPrep = 1, tBake = 10, nCook = 2, nDeliverer = 10, nOvens = 5, averageTime,
	 averageTimeCold, maxTime, maxTimeCold, totalTime, totalTimeCold, deliveryTime;
pthread_mutex_t mutexnCook, mutexnOvens, mutexPrint, mutexnDeliverer;
pthread_cond_t condtPreparation, condBaking, condDeliverer;

void* OrderThread(void* threadId);
