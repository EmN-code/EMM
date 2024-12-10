#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_DEFAULT 2048
#define PRIORITY_HIGH 1
#define PRIORITY_LOW 2

#define MSG_Q_SIZE 8

char stack_1[THREAD_STACKSIZE_DEFAULT];
char stack_2[THREAD_STACKSIZE_DEFAULT];

msg_t msg_queue[MSG_Q_SIZE];

void *thread_1(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    printf("Thread 1: Sending message with value: %d\n", msg.content.value);
    msg_send(&msg, thread_getpid_of((kernel_pid_t)arg));
    return NULL;
}

void *thread_2(void *arg) {
    (void)arg;
    msg_t msg;
    msg_receive(&msg);
    printf("Thread 2: Received message with value: %ld\n", (long int)msg.content.value);
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_Q_SIZE);

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), PRIORITY_LOW, 0, thread_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), PRIORITY_HIGH, 0, thread_2, (void*)pid_1, "thread_2");
    
    while (1) {
        xtimer_sleep(5);
    }
    return 0;
}