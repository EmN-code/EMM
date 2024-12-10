#include <unistd.h>
#include <time_units.h>
#include <thread_arch.h>
#include <thread_config.h>
#include <thread.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <sched.h>
#include <MQTTClient.h>
#include <dht_params.h>
#include <event/thread.h>
#include <architecture.h>

#define THREAD1_STACKSIZE (THREAD_STACKSIZE_MAIN / 2)
#define THREAD2_STACKSIZE (THREAD_STACKSIZE_MAIN / 2)

void *thread1(void *arg) {
    while (1) {
        printf("Thread 1 running with priority %u\n", thread_get_priority(thread_getpid_of(thread1)));
        sleep(1);
    }
    return NULL;
}

void *thread2(void *arg) {
    while (1) {
        printf("Thread 2 running with priority %u\n", thread_get_priority(thread_getpid_of(thread2)));
        sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;
    char stack1[THREAD1_STACKSIZE];
    char stack2[THREAD2_STACKSIZE];

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                        CREATE_STACKTEST, thread1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN,
                        CREATE_STACKTEST, thread2, NULL, "thread2");

    // Let the threads run for a while
    sleep(5);

    // Change the priority of the threads
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 3);

    // Observe the change
    sleep(5);
    
    return 0;
}
