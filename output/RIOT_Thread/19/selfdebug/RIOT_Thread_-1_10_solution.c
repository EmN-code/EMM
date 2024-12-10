#include <thread.h>
#include <stdio.h>
#include "msg.h"

#define THREAD_STACKSIZE 1024

char thread_stack1[THREAD_STACKSIZE];
char thread_stack2[THREAD_STACKSIZE];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread_handler_1(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t reply;

    while (1) {
        msg_receive(&msg);
        printf("Thread 1 received message with type: %u\n", (unsigned)msg.type);

        // Send a reply back to Thread 2
        reply.type = 1;
        msg_reply(&msg, &reply);
    }
    return NULL;
}

void *thread_handler_2(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t reply;
    msg.content.value = 42;
    msg.type = 2;

    while (1) {
        msg_send_receive(&msg, &reply, thread1_pid);
        printf("Thread 2 received reply with type: %u\n", (unsigned)reply.type);

        // Small delay
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    thread1_pid = thread_create(thread_stack1, sizeof(thread_stack1),
                                THREAD_PRIORITY_MAIN - 1, 0, thread_handler_1, NULL, "thread1");

    thread2_pid = thread_create(thread_stack2, sizeof(thread_stack2),
                                THREAD_PRIORITY_MAIN - 2, 0, thread_handler_2, NULL, "thread2");

    return 0;
}