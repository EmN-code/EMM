#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "unistd.h"
#include "time_units.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define THREAD_STACKSIZE_MAIN  (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW    (THREAD_PRIORITY_MAIN - 2)

static char stack_thread_one[THREAD_STACKSIZE_MAIN];
static char stack_thread_two[THREAD_STACKSIZE_MAIN];

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread One running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread Two running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid = thread_create(stack_thread_one, sizeof(stack_thread_one),
                                               THREAD_PRIORITY_LOW,
                                               THREAD_CREATE_STACKTEST,
                                               thread_one, NULL, "thread_one");
    
    kernel_pid_t thread_two_pid = thread_create(stack_thread_two, sizeof(stack_thread_two),
                                               THREAD_PRIORITY_MAIN,
                                               THREAD_CREATE_STACKTEST,
                                               thread_two, NULL, "thread_two");
                                                
    // Change priority of thread one to higher at runtime
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN);
                                                 
    while(1) {
        xtimer_sleep(5);
    }

    return 0;
}