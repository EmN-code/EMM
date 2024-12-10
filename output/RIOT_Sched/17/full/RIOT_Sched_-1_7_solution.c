#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <unistd.h>

#define THREAD_STACKSIZE_DEFAULT (1024)
#define THREAD_PRIORITY_MAIN (5)
#define THREAD_CREATE_STACKTEST (1)

char thread_stack_one[THREAD_STACKSIZE_DEFAULT];
char thread_stack_two[THREAD_STACKSIZE_DEFAULT];
kernel_pid_t thread_one;
kernel_pid_t thread_two;

void *thread_one_handler(void *arg) {
    puts("Thread One is running.");
    while (1) {
        // Simulating task
        sleep(1);
    }
    return NULL;
}

void *thread_two_handler(void *arg) {
    puts("Thread Two is running.");
    while (1) {
        // Simulating task
        sleep(1);
    }
    return NULL;
}

int main(void) {
    puts("Starting RIOT Threads with priority change");

    thread_one = thread_create(thread_stack_one, sizeof(thread_stack_one),
                               THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_STACKTEST,
                               thread_one_handler, NULL, "thread_one");

    thread_two = thread_create(thread_stack_two, sizeof(thread_stack_two),
                               THREAD_PRIORITY_MAIN + 2, THREAD_CREATE_STACKTEST,
                               thread_two_handler, NULL, "thread_two");

    // Changing priority of the threads
    sched_change_priority(thread_get(thread_one), THREAD_PRIORITY_MAIN + 2);
    sched_change_priority(thread_get(thread_two), THREAD_PRIORITY_MAIN + 1);
    return 0;
}