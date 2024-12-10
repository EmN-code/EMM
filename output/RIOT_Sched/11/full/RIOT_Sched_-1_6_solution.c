#include <netinet/in.h>  // Must include network headers if using network functionalities
#include <arpa/inet.h>    // For inet functions

#include "dht.h"         // Must include sensor modules if used
#include "dht_params.h"  // Parameters for sensor
#include "mma8x5x.h"     // Additional sensor libraries
#include "mma8x5x_params.h" // Sensor parameters 
#include "mma8x5x_regs.h" // Sensor registry definitions
#include "xtimer.h"      // Timer functions for RIOT
#include "ztimer.h"      // ZTimer functions for RIOT
#include "time_units.h"  // Time unit helpers

#include "msg.h"         // Message passing among threads
#include "event/thread.h" // For event-related functions
#include "evtimer_mbox.h" // Event timer through mailbox
#include "evtimer_msg.h"  // Event timer messages
#include "kernel_defines.h" // Definitions common across kernel

#include "pthread.h"      // Instead of using native threads, use POSIX threads for abstraction

#include "stdbool.h"     // Standard boolean type
#include "stdatomic.h"   // Atomic operations
#include "stddef.h"      // Standard definitions


/* Redefine the thread priority for example in this case */
#define THREAD_PRIORITY_MAIN (5U)  // example priority value, adjust as necessary

/* Function prototypes */
void *thread1_handler(void *arg);
void *thread2_handler(void *arg);

int main(void) {
    /* Create threads using POSIX threads */
    pthread_t thread1, thread2;
    
    pthread_create(&thread1, NULL, thread1_handler, NULL);
    pthread_create(&thread2, NULL, thread2_handler, NULL);
    
    /* Change Their Priorities */
    struct sched_param sp1, sp2;
    sp1.sched_priority = THREAD_PRIORITY_MAIN + 1;
    sp2.sched_priority = THREAD_PRIORITY_MAIN;
    pthread_setschedparam(thread1, SCHED_FIFO, &sp1);
    pthread_setschedparam(thread2, SCHED_FIFO, &sp2);

    /* Wait for threads to finish */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}

void *thread1_handler(void *arg) {
    while(1) {
        // Perform thread 1 task
        xtimer_usleep(1000000); // Sleep for 1 second
    }
    return NULL;
}

void *thread2_handler(void *arg) {
    while(1) {
        // Perform thread 2 task
        xtimer_usleep(1000000); // Sleep for 1 second
    }
    return NULL;
}
