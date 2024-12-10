
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <unistd.h>

#define STACKSIZE 1024
#define NUM_MESSAGES 8

char stack1[STACKSIZE];
char stack2[STACKSIZE];

msg_t msg_array[NUM_MESSAGES];

void thread_func1(void *arg) {
    (void)arg;
    printf("Thread 1 started\n");
    while (1) {
        printf("Thread 1 running\n");
        sleep(1);
    }
}

void thread_func2(void *arg) {
    (void)arg;
    printf("Thread 2 started\n");
    while (1) {
        printf("Thread 2 running\n");
        sleep(1);
    }
}

int main(void) {
    msg_init_queue(msg_array, NUM_MESSAGES);

    kernel_pid_t pid1 = thread_create(stack1, STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");

    kernel_pid_t pid2 = thread_create(stack2, STACKSIZE, THREAD_PRIORITY_MAIN - 2,
                                      THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    printf("Main thread started\n");

    // Change priority of thread 1 to higher
    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 3);
    printf("Changed priority of thread 1 to higher\n");

    // Change priority of thread 2 to lower
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 1);
    printf("Changed priority of thread 2 to lower\n");

    while (1) {
        printf("Main thread running\n");
        sleep(2);
    }

    return 0;
}
