#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <thread_arch.h>
#include <thread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sched.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <kernel_defines.h>
#include <architecture.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <thread_config.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY_1 (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_2 (THREAD_PRIORITY_MAIN - 2)

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

void *thread1(void *arg) {
    (void)arg;
    for (;;) {
        printf("Thread 1 running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread2(void *arg) {
    (void)arg;
    for (;;) {
        printf("Thread 2 running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_1, 0,
                                thread1, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_2, 0,
                                thread2, NULL, "thread2");

    // Change priority of thread1 to THREAD_PRIORITY_2 after creation
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_2);

    // Change priority of thread2 to THREAD_PRIORITY_1 after creation
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_1);

    // Wait for threads to run
    ztimer_sleep(ZTIMER_MSEC, 5000);

    puts("Main thread: Applications have ended");

    return 0;
}