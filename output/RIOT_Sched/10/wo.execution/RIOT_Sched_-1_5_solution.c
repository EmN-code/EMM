#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <sched.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <MQTTClient.h>
#include <time_units.h>

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)

char thread_stack_one[THREAD_STACKSIZE_MAIN];
char thread_stack_two[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Running thread one\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Running thread two\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    puts("Starting two threads and changing their priorities at runtime\n");

    thread_one_pid = thread_create(thread_stack_one, sizeof(thread_stack_one),
                                   THREAD_PRIORITY_MAIN, 0,
                                   thread_one, NULL, "thread_one");

    thread_two_pid = thread_create(thread_stack_two, sizeof(thread_stack_two),
                                   THREAD_PRIORITY_MAIN + 1, 0,
                                   thread_two, NULL, "thread_two");

    xtimer_sleep(5);

    printf("Changing priority of thread one\n");
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN + 2);

    printf("Changing priority of thread two\n");
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN - 1);

    return 0;
}