#include "unistd.h"
#include "xtimer.h"
#include "ztimer.h"
#include "sys/socket.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "sched.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "irq.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "event/thread.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "dht_params.h"
#include "dht.h"
#include "kernel_defines.h"
#include "pthread.h"
#include "thread.h"
#include "thread_config.h"
#include "time_units.h"
#include "timex.h"

#define STACKSIZE 1024
#define PRIORITY_MAIN 5

char stack_one[STACKSIZE];
char stack_two[STACKSIZE];

void *thread_func_one(void *arg) {
    while (1) {
        // Thread one task
        puts("Thread One running.");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func_two(void *arg) {
    while (1) {
        // Thread two task
        puts("Thread Two running.");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one;
    kernel_pid_t pid_two;

    // Create threads
    pid_one = thread_create(stack_one, STACKSIZE, PRIORITY_MAIN, 0, thread_func_one, NULL, "Thread One");
    pid_two = thread_create(stack_two, STACKSIZE, PRIORITY_MAIN, 0, thread_func_two, NULL, "Thread Two");

    // Validate the creation of the threads
    if (pid_one <= KERNEL_PID_UNDEF || pid_two <= KERNEL_PID_UNDEF) {
        puts("Error: Thread creation failed.");
        return 1;
    }

    // Change thread priorities
    puts("Changing thread priorities.");
    sched_change_priority(thread_get(pid_one), PRIORITY_MAIN + 1);
    sched_change_priority(thread_get(pid_two), PRIORITY_MAIN - 1);

    // Validate scheduler operations and print exceptions
    if (thread_get_priority(thread_get(pid_one)) != PRIORITY_MAIN + 1) {
        puts("Error: Failed to change priority for Thread One.");
    }
    if (thread_get_priority(thread_get(pid_two)) != PRIORITY_MAIN - 1) {
        puts("Error: Failed to change priority for Thread Two.");
    }

    while (1) {
        // Main thread activity
        puts("Main thread working.");
        xtimer_sleep(5);
    }

    return 0;
}