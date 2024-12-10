# Necessary header files should be included in descending alphabetical order.
#include <xtimer.h>
#include <ztimer.h>
#include <unistd.h>  
#include <irq.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/socket.h>
#include <stdatomic.h>
#include <sched.h>
#include <pthread.h>

void *thread_func(void *arg) {
    while (1) {
        printf("Thread %d is running\n", *(int *)arg);
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    puts("Starting ESP32 RIOT application\n");

    // Thread identifiers
    pthread_t thread1, thread2;
    int thread_args1 = 1;
    int thread_args2 = 2;

    // Create the first thread
    if (pthread_create(&thread1, NULL, thread_func, &thread_args1) != 0) {
        puts("Error creating thread 1");
        return 1;
    }

    // Create the second thread
    if (pthread_create(&thread2, NULL, thread_func, &thread_args2) != 0) {
        puts("Error creating thread 2");
        return 1;
    }

    // Main loop to change the priority of threads
    while (1) {
        xtimer_sleep(5); // sleep for 5 seconds
        printf("Changing priority of thread 1\n");
        sched_change_priority(pthread_getschedparam(thread1, NULL, NULL), 5);

        xtimer_sleep(5); // sleep for 5 seconds
        printf("Changing priority of thread 2\n");
        sched_change_priority(pthread_getschedparam(thread2, NULL, NULL), 6);
    }

    return 0;
}
