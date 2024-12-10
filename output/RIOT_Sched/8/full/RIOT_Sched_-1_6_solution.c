#include <xtimer.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/emcute.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sched.h>
#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <irq.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <atomic>
#include <MQTTClient.h>
#include "thread_config.h"
#include <net/sock/util.h>
#include <net/sock/udp.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

/* This is the function that will be executed by the threads */
void *thread_func(void *arg) {
    (void)arg;
    while(1) {
        printf("Thread %p running\n", (void*)thread_getpid());
        xtimer_sleep(1);  // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1, thread2;

    /* Create two threads */
    thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0,
                            thread_func, NULL, "thread1");
    if (thread1 <= KERNEL_PID_UNDEF) {
        printf("Error creating thread 1\n");
        return -1;
    }

    thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 0,
                            thread_func, NULL, "thread2");
    if (thread2 <= KERNEL_PID_UNDEF) {
        printf("Error creating thread 2\n");
        return -1;
    }

    /* Change the priority of thread1 */
    sched_change_priority(thread1, THREAD_PRIORITY_MAIN - 2);

    /* Simulation loop */
    while (1) {
        /* Threads will continue executing their assigned functions */
        xtimer_sleep(5);  // Sleep for 5 seconds

        /* Change thread priority as needed during runtime */
        sched_change_priority(thread2, THREAD_PRIORITY_MAIN - 3);
    }

    return 0;
}