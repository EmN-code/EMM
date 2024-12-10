#include "ztimer.h"
#include "time_units.h"
#include "timex.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "thread.h"
#include "sched.h"
#include "MQTTClient.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"

#define MAIN_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN / 2)

static char stack_thread_one[THREAD_STACKSIZE];
static char stack_thread_two[THREAD_STACKSIZE];

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread One Running\n");
        usleep(1000000); // Sleep for 1 second
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread Two Running\n");
        usleep(1000000); // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one, pid_two;
    
    pid_one = thread_create(stack_thread_one, THREAD_STACKSIZE,
                            THREAD_PRIORITY_MAIN - 1, 0, 
                            thread_one, NULL, "thread_one");
    
    pid_two = thread_create(stack_thread_two, THREAD_STACKSIZE,
                            THREAD_PRIORITY_MAIN - 2, 0, 
                            thread_two, NULL, "thread_two");
    
    usleep(2000000); // Let the threads run for a while
    
    if (pid_one > 0 && pid_two > 0) {
        // Change priorities
        sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 2);
        sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN - 1);
    }
    
    while(1) {
        // Main thread doing nothing, just prevents the program from exiting
    }
    
    return 0;
}