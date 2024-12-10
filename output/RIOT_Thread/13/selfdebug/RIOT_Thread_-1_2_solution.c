#include <xtimer.h>
#include <stdio.h>
#include <thread.h>

#define THREAD_PRIORITY_MAIN 5 // Adjust as per the required priority level for the main thread
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

msg_t msg_queue[8];

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example message content

    while (1) {
        printf("Thread 1: Sending message to Thread 2\n");
        msg_send(&msg, thread2_pid);
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Thread 2: Received message with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread1, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST, thread2, NULL, "thread2");

    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}