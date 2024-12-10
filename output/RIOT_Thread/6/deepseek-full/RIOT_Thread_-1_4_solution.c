
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "thread.h"
#include "msg.h"

#define STACK_SIZE 512
#define MSG_QUEUE_SIZE 16

char sender_stack[STACK_SIZE];
char receiver_stack[STACK_SIZE];

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Example message content

    while (1) {
        msg_send(&msg, receiver_pid);
        printf("Message sent to receiver\n");
        sleep(1); // Sleep for 1 second
    }

    return NULL;
}

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Message received from sender: %u\n", (unsigned int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               THREAD_PRIORITY_MAIN - 1, 0,
                               sender_thread, NULL, "sender");

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_MAIN - 2, 0,
                                 receiver_thread, NULL, "receiver");

    printf("Threads created, starting communication...\n");

    return 0;
}
