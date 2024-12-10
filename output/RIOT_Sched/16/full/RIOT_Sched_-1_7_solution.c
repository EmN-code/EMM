#include <unistd.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sched.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"
#include "dht.h"
#include "thread_config.h"
#include "time_units.h"
#include "thread.h"
#include "thread_arch.h"
#include "esp_system.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "architecture.h"
#include "stdio.h"
#include "mma8x5x_regs.h"
#include "evtimer_msg.h"

#define THREAD_PRIORITY_MAIN 5
#define THREAD_STACKSIZE_MAIN 1024

void *thread_one(void *arg) {
    printf("Thread One running with priority: %d\n", sched_active_thread->priority);
    return NULL;
}

void *thread_two(void *arg) {
    printf("Thread Two running with priority: %d\n", sched_active_thread->priority);
    return NULL;
}

int main(void) {
    char stack_one[THREAD_STACKSIZE_MAIN];
    char stack_two[THREAD_STACKSIZE_MAIN];
    
    // Create threads
    kernel_pid_t thread_one_pid = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");
    kernel_pid_t thread_two_pid = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN + 2, THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");
    
    // Change priority
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN + 3);
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN + 4);

    printf("Main thread running\n");

    // Allow other threads to run
    thread_yield();

    return 0;
}