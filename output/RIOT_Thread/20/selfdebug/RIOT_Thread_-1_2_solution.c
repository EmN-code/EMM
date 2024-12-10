#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_PRIORITY_1 (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_2 (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE  THREAD_STACKSIZE_MAIN

static char stack1[THREAD_STACKSIZE];
static char stack2[THREAD_STACKSIZE];

void *thread_handler1(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    printf("Sending message with content: %lu\n", msg.content.value);
    msg_send(&msg, thread_get((kernel_pid_t)arg));
    return NULL;
}

void *thread_handler2(void *arg) {
    (void)arg;
    msg_t msg;
    msg_receive(&msg);
    printf("Received message with content: %lu\n", msg.content.value);
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;
    
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_2, THREAD_CREATE_STACKTEST, thread_handler2, NULL, "thread2");
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_1, THREAD_CREATE_STACKTEST, thread_handler1, (void *)pid2, "thread1");
    
    return 0;
}