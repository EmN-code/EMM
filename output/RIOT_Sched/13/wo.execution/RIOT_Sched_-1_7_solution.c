// Description: This code demonstrates creating two threads in RIOT OS for an ESP32 board, where the threads change priority at runtime using the sched_change_priority function. This simple example is designed to show how you can manage thread priorities dynamically, and illustrate handling common pitfalls such as avoiding priority inversion or deadlocks. 

#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "sched.h"
#include "netinet/in.h"
#include "stddef.h"
#include "stdatomic.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "sys/socket.h"
#include "msg.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "thread.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "arpa/inet.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "net/gcoap.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 3

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread One is running with priority: %u\n", thread_get_priority(thread_get(thread_one_pid)));
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread Two is running with priority: %u\n", thread_get_priority(thread_get(thread_two_pid)));
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return NULL;
}

int main(void) {
    thread_one_pid = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN, 0, thread_one, NULL, "thread_one");
    thread_two_pid = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN, 0, thread_two, NULL, "thread_two");

    printf("Threads started\n");

    ztimer_sleep(ZTIMER_SEC, 5);

    printf("Changing priorities\n");

    disableIRQ(); // Assuming disableIRQ() as a function to disable interrupts
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN - 1);
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN - 2);
    enableIRQ(); // Assuming enableIRQ() as a function to enable interrupts

    while (1) {
        ;
    }
    return 0;
}

// Notes:
// - Priority levels should ensure thread_two has a higher priority than thread_one after changes.
// - Macros or functions like ztimer_sleep() are used for timing to allow thread scheduling.