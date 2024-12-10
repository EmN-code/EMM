#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <msg.h>
#include <thread.h>

#define THREAD_STACKSIZE 1024
#define THREAD_PRIORITY_MAIN 5
#define THREAD_PRIORITY_LOW 7
#define THREAD_PRIORITY_HIGH 3

static char stack1[THREAD_STACKSIZE];
static char stack2[THREAD_STACKSIZE];

static void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread running with priority: %d\n", thread_get_priority(NULL));
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_LOW, 0, 
                         thread_func, NULL, "thread1");
                         
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_LOW, 0, 
                         thread_func, NULL, "thread2");

    // Let's simulate changing the priority at runtime
    xtimer_sleep(5);  // Allow threads to run for some time
    thread_set_priority(&pid1, THREAD_PRIORITY_HIGH);
    thread_set_priority(&pid2, THREAD_PRIORITY_MAIN);
    
    printf("Changed thread priorities\n");

    // Keep the main function alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}