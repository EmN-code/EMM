#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define THREAD_STACKSIZE_DEFAULT (1024)
#define THREAD_PRIORITY_MAIN 1

#include "msg.h"
#include "thread.h"

msg_t queue[8];
msg_t msg;

kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

char thread_one_stack[THREAD_STACKSIZE_DEFAULT];
char thread_two_stack[THREAD_STACKSIZE_DEFAULT];

void *thread_one(void *arg) {
    (void) arg;
    msg_t msg, reply;
    while (1) {
        msg_receive(&msg);
        printf("Thread One received a message\n");
        msg.content.value = 42;
        msg_send(&msg, thread_two_pid);
        printf("Thread One sent reply\n");
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void) arg;
    msg_t msg;
    msg.content.value = 1;
    printf("Thread Two sending message\n");
    msg_send(&msg, thread_one_pid);
    msg_receive(&msg);
    printf("Thread Two received reply with value: %d\n", msg.content.value);
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);
    
    thread_one_pid = thread_create(thread_one_stack, sizeof(thread_one_stack), THREAD_PRIORITY_MAIN - 1, 0, thread_one, NULL, "thread_one");
    thread_two_pid = thread_create(thread_two_stack, sizeof(thread_two_stack), THREAD_PRIORITY_MAIN - 2, 0, thread_two, NULL, "thread_two");

    while (1) {}

    return 0;
}