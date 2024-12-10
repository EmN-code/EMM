#include <thread_config.h>
#include <thread_arch.h>
#include <net/protnum.h>
#include <architecture.h>

#define THREAD_STACKSIZE_MAIN    (2 * THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread_function1(void *arg) {
    while(1) {
        printf("Thread 1 is running\n");
        thread_yield();
    }
    return NULL;
}

void *thread_function2(void *arg) {
    while(1) {
        printf("Thread 2 is running\n");
        thread_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t t1, t2;

    t1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0,
                       thread_function1, NULL, "thread1");

    t2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 1, 0,
                       thread_function2, NULL, "thread2");

    printf("Threads created\n");

    while(1) {
        printf("Changing thread priority\n");
        sched_change_priority(t1, THREAD_PRIORITY_MAIN + 1);
        sched_change_priority(t2, THREAD_PRIORITY_MAIN);
        thread_yield_higher();  // Yield to higher priority thread
    }

    return 0;
}