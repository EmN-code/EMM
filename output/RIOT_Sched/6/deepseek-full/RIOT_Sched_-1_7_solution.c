
#include <stdio.h>
#include <thread.h>
#include <sched.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <msg.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <net/sock/util.h>
#include <MQTTClient.h>
#include <net/sock/udp.h>
#include <sys/socket.h>
#include <mbox.h>
#include <arpa/inet.h>
#include <mma8x5x_regs.h>
#include <net/emcute.h>
#include <unistd.h>
#include <stdatomic.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <mma8x5x_params.h>

#define THREAD_STACKSIZE_DEFAULT (512)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

void *thread_func_1(void *arg) {
    printf("Thread 1 running\n");
    thread_yield();
    printf("Thread 1 done\n");
    return NULL;
}

void *thread_func_2(void *arg) {
    printf("Thread 2 running\n");
    thread_yield();
    printf("Thread 2 done\n");
    return NULL;
}

int main(void) {
    char stack_1[THREAD_STACKSIZE_DEFAULT];
    char stack_2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN, 0, thread_func_2, NULL, "thread_2");

    printf("Threads created, pid_1: %d, pid_2: %d\n", pid_1, pid_2);

    // Wait for threads to finish
    thread_sleep();

    // Change priority of thread 1
    sched_change_priority(pid_1, THREAD_PRIORITY_MAIN + 1);
    printf("Priority of thread 1 changed\n");

    // Change priority of thread 2
    sched_change_priority(pid_2, THREAD_PRIORITY_MAIN + 2);
    printf("Priority of thread 2 changed\n");

    return 0;
}
