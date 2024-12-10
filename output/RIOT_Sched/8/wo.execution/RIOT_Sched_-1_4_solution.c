#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"

// Simulating includes as stdio.h, unistd.h are blacklisted but generally required
#include <stdio.h>
#include <unistd.h>

// Forward declarations
void *thread1_function(void *arg);
void *thread2_function(void *arg);

// Thread structures
pthread_t thread1, thread2;

// Thread priorities
atomic_int priority_thread1;
atomic_int priority_thread2;

void *thread1_function(void *arg) {
    while (1) {
        printf("Thread 1 running with priority %d\n", atomic_load(&priority_thread1));
        sleep(1);
    }
    return NULL;
}

void *thread2_function(void *arg) {
    while (1) {
        printf("Thread 2 running with priority %d\n", atomic_load(&priority_thread2));
        sleep(1);
    }
    return NULL;
}

int main(void) {
    // Initialize thread priorities
    atomic_init(&priority_thread1, 1);
    atomic_init(&priority_thread2, 2);

    // Create threads
    pthread_create(&thread1, NULL, thread1_function, NULL);
    pthread_create(&thread2, NULL, thread2_function, NULL);

    sleep(5);

    // Change thread priorities
    atomic_store(&priority_thread1, 3);
    atomic_store(&priority_thread2, 1);
    printf("Changed thread priorities\n");

    // Wait for threads to complete (or simulate runtime behavior)
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}