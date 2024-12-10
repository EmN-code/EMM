#include <thread.h>
#include <stdio.h>
#include <xtimer.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

char thread_stack_one[THREAD_STACKSIZE_MAIN];
char thread_stack_two[THREAD_STACKSIZE_MAIN];

void *thread_func_one(void *arg) {
    (void)arg;
    
    while (1) {
        printf("Thread One is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func_two(void *arg) {
    (void)arg;
    
    while (1) {
        printf("Thread Two is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    puts("Starting thread priority change example\n");

    thread_create(thread_stack_one, sizeof(thread_stack_one), THREAD_PRIORITY_MAIN, 0, thread_func_one, NULL, "thread_one");
    thread_create(thread_stack_two, sizeof(thread_stack_two), THREAD_PRIORITY_MAIN + 1, 0, thread_func_two, NULL, "thread_two");

    xtimer_sleep(5);

    // Change the priority of the threads
    thread_set_priority(thread_getpid(), THREAD_PRIORITY_MAIN + 1);
    printf("Changed Thread One priority\n");
    thread_set_priority(thread_getpid(), THREAD_PRIORITY_MAIN);
    printf("Changed Thread Two priority\n");

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}