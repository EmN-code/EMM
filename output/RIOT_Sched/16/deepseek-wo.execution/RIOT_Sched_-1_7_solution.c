
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <stdatomic.h>
#include <stddef.h>
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

#define STACKSIZE 1024
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

void thread_func_1(void *arg)
{
    (void)arg;
    printf("Thread 1 running at priority %d\n", sched_active_thread_priority());
    while (1) {
        usleep(1000000); // Sleep for 1 second
    }
}

void thread_func_2(void *arg)
{
    (void)arg;
    printf("Thread 2 running at priority %d\n", sched_active_thread_priority());
    while (1) {
        usleep(1000000); // Sleep for 1 second
    }
}

int main(void)
{
    char stack_1[STACKSIZE];
    char stack_2[STACKSIZE];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), PRIORITY_HIGH, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), PRIORITY_LOW, 0, thread_func_2, NULL, "thread_2");

    printf("Main thread running\n");

    // Change priority of thread 1 to low after 5 seconds
    xtimer_sleep(5);
    sched_change_priority(pid_1, PRIORITY_LOW);
    printf("Changed priority of thread 1 to %d\n", PRIORITY_LOW);

    // Change priority of thread 2 to high after 10 seconds
    xtimer_sleep(5);
    sched_change_priority(pid_2, PRIORITY_HIGH);
    printf("Changed priority of thread 2 to %d\n", PRIORITY_HIGH);

    return 0;
}
