#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE (8)
#define THREAD_STACKSIZE_DEFAULT (1024)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN - 2)

static msg_t msg_queue[MAIN_QUEUE_SIZE];

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_func_high(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        if (msg_receive(&msg)) {
            printf("High priority thread received message: %d\n", msg.content.value);
        }
    }
    return NULL;
}

void *thread_func_low(void *arg) {
    (void)arg;
    msg_t msg;
    /* Initialize message content */
    msg.content.value = 42;
    while (1) {
        printf("Low priority thread sending message: %d\n", msg.content.value);
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);
    
    kernel_pid_t pid_high = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread_func_high, NULL, "high");
    kernel_pid_t pid_low = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func_low, NULL, "low");

    printf("Main thread running. High PID: %d, Low PID: %d\n", pid_high, pid_low);

    return 0;
}