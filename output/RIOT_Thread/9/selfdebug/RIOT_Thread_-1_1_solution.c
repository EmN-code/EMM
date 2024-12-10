#include <ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SECOND (THREAD_PRIORITY_MAIN - 2)

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

void *thread1(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example message content
    kernel_pid_t target_pid = (kernel_pid_t)arg;

    while(1) {
        printf("Thread 1: Sending message\n");
        msg_send(&msg, target_pid);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return NULL;
}

void *thread2(void *arg) {
    (void)arg;
    msg_t msg;

    while(1) {
        if (msg_receive(&msg) == 1) {
            printf("Thread 2: Received message with value: %d\n", msg.content.value);
        }
    }

    return NULL;
}

int main(void) {
    kernel_pid_t main_pid = thread_create(stack1, THREAD_STACKSIZE,
                                          THREAD_PRIORITY_MAIN, 0,
                                          thread1, (void *)(kernel_pid_t)thread_getpid(), "thread1");

    kernel_pid_t second_pid = thread_create(stack2, THREAD_STACKSIZE,
                                            THREAD_PRIORITY_SECOND, 0,
                                            thread2, NULL, "thread2");

    // Keep the main thread running
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}