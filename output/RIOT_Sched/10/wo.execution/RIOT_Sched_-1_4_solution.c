#include<ztimer.h>
#include<xtimer.h>
#include<unistd.h>
#include<timex.h>
#include<time_units.h>
#include<stddef.h>
#include<stdatomic.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<msg.h>
#include<mbox.h>
#include<MQTTClient.h>
#include<paho_mqtt.h>
#include<sched.h>
#include<thread.h>
#include<evtimer_msg.h>
#include<evtimer_mbox.h>
#include<event/thread.h>

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)

static char stack_one[THREAD_STACKSIZE_MAIN];
static char stack_two[THREAD_STACKSIZE_MAIN];

void *thread_one_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread one is running at priority %d\n", thread_get_priority(thread_getpid()));
        sleep(1);
    }
    return NULL;
}

void *thread_two_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread two is running at priority %d\n", thread_get_priority(thread_getpid()));
        sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one = thread_create(stack_one, sizeof(stack_one),
                                            THREAD_PRIORITY_MAIN + 1, CREATE_STACKTEST,
                                            thread_one_func, NULL, "thread_one");

    kernel_pid_t thread_two = thread_create(stack_two, sizeof(stack_two),
                                            THREAD_PRIORITY_MAIN + 2, CREATE_STACKTEST,
                                            thread_two_func, NULL, "thread_two");

    // Allow some time to run before changing priority
    sleep(3);

    // Changing priorities at runtime
    sched_change_priority(thread_get(thread_one), THREAD_PRIORITY_MAIN + 2);
    sched_change_priority(thread_get(thread_two), THREAD_PRIORITY_MAIN + 1);

    while (1) {
        // Main thread does something
        printf("Main thread is active\n");
        sleep(2);
    }
    return 0;
}