
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
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

void *thread_func_1(void *arg) {
    (void)arg;
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread_func_2(void *arg) {
    (void)arg;
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void) {
    char stack_1[STACKSIZE];
    char stack_2[STACKSIZE];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), PRIORITY_HIGH, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), PRIORITY_LOW, 0, thread_func_2, NULL, "thread_2");

    printf("Threads created: PID 1 = %d, PID 2 = %d\n", pid_1, pid_2);

    xtimer_sleep(2); // Wait for threads to run

    sched_change_priority(thread_get(pid_1), PRIORITY_LOW);
    sched_change_priority(thread_get(pid_2), PRIORITY_HIGH);

    printf("Thread priorities changed\n");

    return 0;
}
