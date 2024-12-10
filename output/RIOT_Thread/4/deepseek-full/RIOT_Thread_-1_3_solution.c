
#include <stdio.h>
#include <stdint.h>
#include "thread.h"
#include "msg.h"

#define STACKSIZE 512
#define NUM_MESSAGES 8

char stack1[STACKSIZE];
char stack2[STACKSIZE];

kernel_pid_t pid1, pid2;

void *thread1(void *arg)
{
    msg_t msg_queue[NUM_MESSAGES];
    msg_init_queue(msg_queue, NUM_MESSAGES);

    msg_t msg;
    msg.type = 1;
    msg.content.value = 123;

    printf("Thread 1 sending message to Thread 2\n");
    msg_send_receive(&msg, &msg, pid2);

    printf("Thread 1 received reply: %lu\n", (unsigned long)msg.content.value);

    return NULL;
}

void *thread2(void *arg)
{
    msg_t msg_queue[NUM_MESSAGES];
    msg_init_queue(msg_queue, NUM_MESSAGES);

    msg_t msg, reply;
    msg_receive(&msg);

    printf("Thread 2 received message: %lu\n", (unsigned long)msg.content.value);

    reply.type = 2;
    reply.content.value = 456;

    printf("Thread 2 replying to Thread 1\n");
    msg_reply(&msg, &reply);

    return NULL;
}

int main(void)
{
    pid1 = thread_create(stack1, STACKSIZE, 1, 0, thread1, NULL, "thread1");
    pid2 = thread_create(stack2, STACKSIZE, 2, 0, thread2, NULL, "thread2");

    return 0;
}
