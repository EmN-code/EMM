#include <stdio.h>
#include <thread.h>
#include <xtimer.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_SECONDARY (THREAD_PRIORITY_MAIN - 2)

char thread_stack_one[STACKSIZE];
char thread_stack_two[STACKSIZE];

void *thread_handler_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running with high priority\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_handler_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running with low priority\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    printf("Starting threads and changing priority at runtime\n");
    
    thread_create(thread_stack_one, sizeof(thread_stack_one), PRIORITY_MAIN, 0, thread_handler_one, NULL, "thread_one");
    thread_create(thread_stack_two, sizeof(thread_stack_two), PRIORITY_SECONDARY, 0, thread_handler_two, NULL, "thread_two");

    xtimer_sleep(5);
    
    printf("Changing priorities\n");
    
    /* Changing the priority of threads dynamically */
    thread_set_priority(thread_getpid_by_name("thread_one"), PRIORITY_SECONDARY);
    thread_set_priority(thread_getpid_by_name("thread_two"), PRIORITY_MAIN);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
