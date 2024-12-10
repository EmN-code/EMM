#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"
#include "atomic.h"
#include "architecture.h"
#include "sched.h"
#include "stdbool.h"
#include "msg.h"
#include "net/emcute.h"
#include "pthread.h"
#include "thread.h"
#include "kernel_defines.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "time_units.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "timex.h"
#include "arpa/inet.h"
#include "evtimer_mbox.h"
#include "stdatomic.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include "thread_arch.h"
#include "pthread_create"
#include "pthread_join"

// Thread stack sizes
#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_STACKSIZE_AUX 1024

// Thread priorities
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_AUX (THREAD_PRIORITY_MAIN - 1)

// Function declaration
void *thread_handler(void *arg);

// Variable definition
static char stack_aux[THREAD_STACKSIZE_AUX];
static char stack_main[THREAD_STACKSIZE_MAIN];

int main(void) {
    puts("Starting threads on ESP32");

    pthread_t auxiliary_thread;
    pthread_attr_t attr;

    // Create the auxiliary thread
    pthread_attr_init(&attr);
    pthread_attr_setstack(&attr, stack_aux, sizeof(stack_aux));
    pthread_create(&auxiliary_thread, &attr, thread_handler, NULL);

    // Changing thread priority
    thread_t *main_thread = (thread_t *)pthread_self();
    sched_change_priority(main_thread, THREAD_PRIORITY_MAIN);

    thread_yield();  // Yield to allow other threads to operate

    puts("Threads are now active and running.");

    // Join the auxiliary thread (waits for it to finish)
    pthread_join(auxiliary_thread, NULL);

    puts("Program completed.");
    return 0;
}

// Simple thread handler function
void *thread_handler(void *arg) {
    (void)arg;
    puts("Auxiliary thread running.");
    ztimer_sleep_usec(1000000);  // Sleep for 1 second using ztimer
    puts("Auxiliary thread finished execution.");
    return NULL;
}