
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <stddef.h>
#include <stdatomic.h>
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

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

void *thread_func1(void *arg) {
    (void)arg;
    printf("Thread 1 running\n");
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    printf("Thread 2 running\n");
    return NULL;
}

int main(void) {
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), PRIORITY, 0, thread_func1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), PRIORITY, 0, thread_func2, NULL, "thread2");

    // Change priority of thread 1
    thread_set_priority(pid1, THREAD_PRIORITY_MAIN - 2);

    // Change priority of thread 2
    thread_set_priority(pid2, THREAD_PRIORITY_MAIN - 3);

    printf("Main thread running\n");

    return 0;
}
