#include "thread_arch.h"
#include "thread_config.h"
#include <stdio.h>
#include <stdint.h>

// Thread stack size, assuming the same size is sufficient for both threads
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

// Thread priorities
#define THREAD_ONE_PRIO (THREAD_PRIORITY_MAIN - 1)
#define THREAD_TWO_PRIO (THREAD_PRIORITY_MAIN - 2)

// Thread handler function declarations
void *thread_one_handler(void *arg);
void *thread_two_handler(void *arg);

// Thread stack declarations
static char thread_one_stack[THREAD_STACKSIZE];
static char thread_two_stack[THREAD_STACKSIZE];

// Thread declaration
static kernel_pid_t thread_one_pid;
static kernel_pid_t thread_two_pid;

int main(void) {
    printf("Starting threads...\n");

    // Create Thread One
    thread_one_pid = thread_create(thread_one_stack, sizeof(thread_one_stack),
                                   THREAD_ONE_PRIO, THREAD_CREATE_STACKTEST,
                                   thread_one_handler, NULL, "ThreadOne");
    
    // Create Thread Two
    thread_two_pid = thread_create(thread_two_stack, sizeof(thread_two_stack),
                                   THREAD_TWO_PRIO, THREAD_CREATE_STACKTEST,
                                   thread_two_handler, NULL, "ThreadTwo");

    // Change priority of Thread One to a lower priority
    printf("Changing ThreadOne priority\n");
    sched_change_priority(thread_one_pid, THREAD_TWO_PRIO);

    return 0;
}

void *thread_one_handler(void *arg) {
    while(1) {
        printf("Thread One is running\n");
        // Add a delay or some processing
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_two_handler(void *arg) {
    while(1) {
        printf("Thread Two is running\n");
        // Add a delay or some processing
        xtimer_sleep(1);
    }
    return NULL;
}