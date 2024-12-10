
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <stddef.h>
#include <stdatomic.h>
#include <sched.h>
#include <paho_mqtt.h>
#include <stdlib.h>
#include <event/thread.h>
#include <ztimer.h>
#include <netinet/in.h>
#include <xtimer.h>
#include <msg.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <thread.h>
#include <mbox.h>
#include <stdbool.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <kernel_defines.h>

#define STACKSIZE 512
#define PRIORITY_LOW 5
#define PRIORITY_HIGH 3

void *thread_low_priority(void *arg) {
    (void)arg;
    printf("Low priority thread running\n");
    return NULL;
}

void *thread_high_priority(void *arg) {
    (void)arg;
    printf("High priority thread running\n");
    return NULL;
}

int main(void) {
    char stack_low[STACKSIZE];
    char stack_high[STACKSIZE];

    kernel_pid_t pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW, 0, thread_low_priority, NULL, "low_prio");
    kernel_pid_t pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, 0, thread_high_priority, NULL, "high_prio");

    xtimer_sleep(2); // Wait for threads to start

    thread_t *low_thread = thread_get(pid_low);
    thread_t *high_thread = thread_get(pid_high);

    sched_change_priority(low_thread, PRIORITY_HIGH);
    sched_change_priority(high_thread, PRIORITY_LOW);

    printf("Priorities swapped\n");

    return 0;
}
