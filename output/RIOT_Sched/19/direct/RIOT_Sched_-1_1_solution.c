#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LOW_PRIORITY 1
#define HIGH_PRIORITY 2

pthread_t thread1, thread2;

void *thread_func1(void *arg) {
    printf("Thread 1 is running with low priority\n");
    // Simulate workload
    for (int i = 0; i < 5; i++) {
        printf("Thread 1 iteration %d\n", i);
    }
    // Change priority to high
    struct sched_param schedParam;
    schedParam.sched_priority = HIGH_PRIORITY;
    if (pthread_setschedparam(thread1, SCHED_FIFO, &schedParam) != 0) {
        perror("pthread_setschedparam");
        exit(EXIT_FAILURE);
    }
    printf("Thread 1 priority changed to high\n");
    return NULL;
}

void *thread_func2(void *arg) {
    printf("Thread 2 is running with low priority\n");
    // Simulate workload
    for (int i = 0; i < 5; i++) {
        printf("Thread 2 iteration %d\n", i);
    }
    // Change priority to high
    struct sched_param schedParam;
    schedParam.sched_priority = HIGH_PRIORITY;
    if (pthread_setschedparam(thread2, SCHED_FIFO, &schedParam) != 0) {
        perror("pthread_setschedparam");
        exit(EXIT_FAILURE);
    }
    printf("Thread 2 priority changed to high\n");
    return NULL;
}

int main(void) {
    struct sched_param schedParam;

    // Create thread 1
    if (pthread_create(&thread1, NULL, thread_func1, NULL) != 0) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }
    // Set initial low priority for thread 1
    schedParam.sched_priority = LOW_PRIORITY;
    if (pthread_setschedparam(thread1, SCHED_FIFO, &schedParam) != 0) {
        perror("pthread_setschedparam");
        return EXIT_FAILURE;
    }

    // Create thread 2
    if (pthread_create(&thread2, NULL, thread_func2, NULL) != 0) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }
    // Set initial low priority for thread 2
    schedParam.sched_priority = LOW_PRIORITY;
    if (pthread_setschedparam(thread2, SCHED_FIFO, &schedParam) != 0) {
        perror("pthread_setschedparam");
        return EXIT_FAILURE;
    }

    // Join threads
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return EXIT_SUCCESS;
}