// To solve the errors faced, the main issue is that in the sched_change_priority function, we have to pass a thread pointer (type listed in the documentation). 
// Let's make sure that we are doing this correctly by using the function 'thread_get' which returns a pointer to the thread structure, given a kernel process id

#include <unistd.h>  // Includes for RIOT API use
#include <time_units.h> 
#include <thread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <sched.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <MQTTClient.h>

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_func(void *arg) {
    (void)arg; // Suppress unused warning
    while (1) {
        // emulate some processing
        usleep(1000000);
    }
    return NULL;
}

int main(void) {
    // Create two threads
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_func, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread_func, NULL, "thread2");

    // The error was related to incorrect argument being passed to sched_change_priority
    // As per the function definition, we need to pass a thread object (pointer), not the PID
    
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 4);
    
    // Keep main thread doing something
    while (1) {
        usleep(1000000);
    }
    return 0;
}