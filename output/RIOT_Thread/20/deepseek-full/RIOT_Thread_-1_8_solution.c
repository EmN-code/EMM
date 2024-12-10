
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread.h>
#include <msg.h>

#define STACKSIZE 1024
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 5

kernel_pid_t high_priority_thread_pid;
kernel_pid_t low_priority_thread_pid;

void *high_priority_thread(void *arg) {
    msg_t msg, reply;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %lu\n", msg.content.value);
        reply.content.value = msg.content.value * 2;
        msg_reply(&msg, &reply);
    }
    return NULL;
}

void *low_priority_thread(void *arg) {
    msg_t msg, reply;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %lu\n", msg.content.value);
        reply.content.value = msg.content.value * 3;
        msg_reply(&msg, &reply);
    }
    return NULL;
}

int main(void) {
    msg_t msg, reply;
    char high_stack[STACKSIZE];
    char low_stack[STACKSIZE];

    high_priority_thread_pid = thread_create(high_stack, sizeof(high_stack), PRIORITY_HIGH, 0, high_priority_thread, NULL, "high_priority_thread");
    low_priority_thread_pid = thread_create(low_stack, sizeof(low_stack), PRIORITY_LOW, 0, low_priority_thread, NULL, "low_priority_thread");

    msg.content.value = 10;
    msg_send_receive(&msg, &reply, high_priority_thread_pid);
    printf("Main thread received reply from high priority thread: %lu\n", reply.content.value);

    msg.content.value = 20;
    msg_send_receive(&msg, &reply, low_priority_thread_pid);
    printf("Main thread received reply from low priority thread: %lu\n", reply.content.value);

    return 0;
}
