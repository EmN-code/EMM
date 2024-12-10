#include <kernel_defines.h>
#include <stddef.h>
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define THREAD_PRIORITY_MAIN 5
#define THREAD_STACKSIZE_MAIN THREAD_STACKSIZE_DEFAULT

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread_handler_1(void *arg) {
    (void)arg;
    msg_t msg;
    char *text = "Hello from Thread 1";
    msg.content.ptr = (void *)text;
    msg_send(&msg, thread_getpid_of((thread_t *)arg));
    return NULL;
}

void *thread_handler_2(void *arg) {
    (void)arg;
    msg_t msg;
    msg_receive(&msg);
    printf("Received message: %s\n", (char *)msg.content.ptr);
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread_handler_1, (void *)&pid2, "thread_1");

    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN,
                         THREAD_CREATE_STACKTEST, thread_handler_2, NULL, "thread_2");

    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}