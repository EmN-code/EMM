
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE 512

char thread_stack1[THREAD_STACKSIZE];
char thread_stack2[THREAD_STACKSIZE];

kernel_pid_t thread_id1, thread_id2;

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    puts("Starting two threads with different priorities...");

    // Create the first thread with a lower priority
    thread_id1 = thread_create(thread_stack1, sizeof(thread_stack1),
                               THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                               thread_func1, NULL, "thread1");

    // Create the second thread with a higher priority
    thread_id2 = thread_create(thread_stack2, sizeof(thread_stack2),
                               THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                               thread_func2, NULL, "thread2");

    // Wait for 10 seconds
    xtimer_sleep(10);

    puts("Changing priorities...");

    // Change the priority of thread 1 to be higher than thread 2
    thread_change_priority(thread_id1, THREAD_PRIORITY_MAIN + 1);

    // Change the priority of thread 2 to be lower than thread 1
    thread_change_priority(thread_id2, THREAD_PRIORITY_MAIN - 1);

    // Wait for another 10 seconds to observe the changes
    xtimer_sleep(10);

    puts("End of main function");
    return 0;
}
