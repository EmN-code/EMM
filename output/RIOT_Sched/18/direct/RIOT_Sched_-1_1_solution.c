#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_A        (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_B        (THREAD_PRIORITY_MAIN - 2)

char thread_stack_a[THREAD_STACKSIZE_MAIN];
char thread_stack_b[THREAD_STACKSIZE_MAIN];

void *thread_a(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread A is running");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_b(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread B is running");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    thread_create(thread_stack_a, sizeof(thread_stack_a), THREAD_PRIORITY_A, 0,
                  thread_a, NULL, "thread_a");
    thread_create(thread_stack_b, sizeof(thread_stack_b), THREAD_PRIORITY_B, 0,
                  thread_b, NULL, "thread_b");

    /* Simulating a change in priority for demonstration */
    int new_priority_a = THREAD_PRIORITY_B;
    int new_priority_b = THREAD_PRIORITY_A;

    /* Assuming somehow dynamically changing priority, 
       typically you would need system calls or additional libraries */
    thread_set_priority(thread_getpid(), new_priority_a);
    thread_set_priority(thread_getpid(), new_priority_b);

    while (1) {
        xtimer_sleep(5);
        puts("Main thread - simulation ongoing");
    }

    return 0;
}
