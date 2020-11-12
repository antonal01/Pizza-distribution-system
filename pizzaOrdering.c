#include "pizzaOrdering.h"

void* OrderThread(void* threadId) {
	
    int* tid;
    unsigned int* ptr = &globalSeed;
    int rc , numberOfPizzas = rand_r(ptr) % (5 + 1 - 1) + 1,             //6 // ((nOrderHigh +1 -nOrderLow) + nOrderlow)
        tPrepTime = (numberOfPizzas * tPrep), tBakeTime = tBake;
    struct timespec start, finish, startGettingCold;
    double elapsed, elapsedCold;
	
	clock_gettime(CLOCK_REALTIME, &start);
	tid = (int*)threadId;

    rc = pthread_mutex_lock(&mutexnCook);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_lock(&mutexnCook) is %d\n", rc);
        exit(-1);
    }
    while (nCook == 0) {
        printf("The order with id [%d] is waiting for a maker...\n", *tid);
        rc = pthread_cond_wait(&condtPreparation, &mutexnCook);
    }

    nCook--;
    rc = pthread_mutex_unlock(&mutexnCook);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_unlock(&mutexnCook) is %d\n", rc);
        exit(-1);
    }

    sleep(tPrepTime);

	//When this happens, the maker places all the ordered pizzas in the same oven,
	//and is released for the next order.
	//(does not wait for baking to finish)
    
    rc = pthread_mutex_lock(&mutexnOvens);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_lock(&mutexnOvens) is %d\n", rc);
        exit(-1);
    }
    while (nOvens == 0) {
        printf("The order with id [%d] is waiting for an oven...\n", *tid);
        rc = pthread_cond_wait(&condBaking, &mutexnOvens);
    }

    nOvens--;
    nCook++;
    rc = pthread_cond_broadcast(&condtPreparation);
    rc = pthread_mutex_unlock(&mutexnOvens);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_unlock(&mutexnOvens) is %d\n", rc);
        exit(-1);
    }
	
    //The pizzas are baked for Tbake time.	
    sleep(tBakeTime);
    clock_gettime(CLOCK_REALTIME, &startGettingCold);

    rc = pthread_mutex_lock(&mutexnDeliverer);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_lock(&mutexnDeliverer) is %d\n", rc);
        exit(-1);
    }
    //Oven closes
    while (nDeliverer == 0) {
        printf("The order with id [%d] is waiting for a deliverer...\n", *tid);
        rc = pthread_cond_wait(&condDeliverer, &mutexnDeliverer);
    }
    nDeliverer--;
    nOvens++;
    rc = pthread_cond_broadcast(&condBaking);

    rc = pthread_mutex_unlock(&mutexnDeliverer);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_unlock(&mutexnDeliverer) is %d\n", rc);
        exit(-1);
    }

    deliveryTime = rand_r(ptr) % (15 + 1 - 5) + 5;// ((tHigh +1 -tLow) + tLow)

    sleep(deliveryTime);
    clock_gettime(CLOCK_REALTIME, &finish);
    elapsed = finish.tv_sec - start.tv_sec;
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    elapsedCold = finish.tv_sec - startGettingCold.tv_sec;
    elapsedCold += (finish.tv_nsec - startGettingCold.tv_nsec) / 1000000000.0;
    //Delivered to the customer
    //Returning back
    sleep(deliveryTime);
    rc = pthread_mutex_lock(&mutexnDeliverer);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_lock(&mutexnDeliverer) is %d\n", rc);
        exit(-1);
    }
    nDeliverer++;
    rc = pthread_cond_broadcast(&condDeliverer);
    rc = pthread_mutex_unlock(&mutexnDeliverer);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_unlock(&mutexnDeliverer) is %d\n", rc);
        exit(-1);
    }

    rc = pthread_mutex_lock(&mutexPrint);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_lock(&mutexPrint) is %d\n", rc);
        exit(-1);
    }

    if (elapsed >= maxTime) {
        maxTime = elapsed;
    }
    if (elapsedCold >= maxTimeCold) {
        maxTimeCold = elapsedCold;
    }
    totalTime = totalTime + elapsed;
    totalTimeCold = totalTimeCold + elapsedCold;
    printf("Order with id number <%d> delivered in <%lf> minutes and being cold for <%lf>  minutes.\n", *tid, elapsed, elapsedCold);
    rc = pthread_mutex_unlock(&mutexPrint);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_unlock(&mutexPrint) is %d\n", rc);
        exit(-1);
    }

    pthread_exit(tid);
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("ERROR: the program should take two arguments!\n");
        exit(-1);
    }

    globalSeed = atoi(argv[2]);
    int ncust = atoi(argv[1]);
    printf("There are %d customers waiting to eat pizza.\n", ncust);
    if (ncust < 0) {
        printf("ERROR: the number of threads to run should be a positive number. Current number given %d.\n", ncust);
        exit(-1);
    }

    pthread_t* threads;
    threads = malloc(ncust * sizeof(pthread_t));
    if (threads == NULL) {
        printf("NOT ENOUGH MEMORY!\n");
        return -1;
    }
    int rc;
    int threadCount;
    int countArray[ncust];

    rc = pthread_mutex_init(&mutexnCook, NULL);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
        exit(-1);
    }
    rc = pthread_mutex_init(&mutexnOvens, NULL);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
        exit(-1);
    }
    rc = pthread_mutex_init(&mutexPrint, NULL);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
        exit(-1);
    }
    rc = pthread_mutex_init(&mutexnDeliverer, NULL);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
        exit(-1);
    }

    pthread_cond_init(&condtPreparation, NULL);
    pthread_cond_init(&condBaking, NULL);
    pthread_cond_init(&condDeliverer, NULL);

    for (threadCount = 0; threadCount < ncust; threadCount++) {

        countArray[threadCount] = threadCount + 1;
        /*Creating threads*/
        rc = pthread_create(&threads[threadCount], NULL, OrderThread, &countArray[threadCount]);
        /*Checking if threads were created right.*/
        if (rc != 0) {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        if (threadCount != ncust) {
            sleep(rand_r(&globalSeed) % (5 + 1 - 1) + 1);//((tOrderHigh +1 -tOrderLow) + tOrderlow)
        }
    }

    void* status;

	/*Necessary to stop the thread, if not set there is a chance that the father will end before the threads and we will not have the desired result*/
    for (threadCount = 0; threadCount < ncust; threadCount++) {
        rc = pthread_join(threads[threadCount], &status);
        if (rc != 0) {
            printf("ERROR: return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }
    averageTime = totalTime / ncust;
    averageTimeCold = totalTimeCold / ncust;
    printf("Average delivery time: %d \nMaximum delivery time: %d\n", averageTime, maxTime);
    printf("Average cold time: %d \nMaximum cold time: %d\n", averageTimeCold, maxTimeCold);

    rc = pthread_mutex_destroy(&mutexnCook);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
        exit(-1);
    }
    rc = pthread_mutex_destroy(&mutexnOvens);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
        exit(-1);
    }
    rc = pthread_mutex_destroy(&mutexnDeliverer);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
        exit(-1);
    }
    rc = pthread_mutex_destroy(&mutexPrint);
    if (rc != 0) {
        printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
        exit(-1);
    }

    pthread_cond_destroy(&condtPreparation);
    pthread_cond_destroy(&condBaking);
    pthread_cond_destroy(&condDeliverer);

    free(threads);
    printf("\nPress Enter to exit.");
    getchar();
    return 1;
}
