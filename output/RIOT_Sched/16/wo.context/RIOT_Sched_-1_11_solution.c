#include <thread.h>
#include <stdio.h>

#define THREAD_STACKSIZE_LARGE (1024 * 3) // Define stack size properly to avoid redefinition
#define THREAD_PRIORITY_DEFAULT (THREAD_PRIORITY_MAIN - 1)

char thread_stack1[THREAD_STACKSIZE_LARGE];
char thread_stack2[THREAD_STACKSIZE_LARGE];

void *thread_handler(void *arg) {
    (void)arg;
    printf("Thread %d is running\n", (int)thread_getpid());
    return NULL;
}

int main(void) {
    kernel_pid_t thread1, thread2;

    // Creating two threads
    thread1 = thread_create(thread_stack1, sizeof(thread_stack1), THREAD_PRIORITY_DEFAULT,
                            CREATE_WOUT_YIELD, thread_handler, NULL, "Thread 1");
    thread2 = thread_create(thread_stack2, sizeof(thread_stack2), THREAD_PRIORITY_DEFAULT,
                            CREATE_WOUT_YIELD, thread_handler, NULL, "Thread 2");

    // Change the priority of the threads after creation
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 3);

    return 0;
}