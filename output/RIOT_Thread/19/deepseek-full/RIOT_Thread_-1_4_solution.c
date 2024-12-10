
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <time_units.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>

#include "thread.h"
#include "msg.h"

#define STACKSIZE 1024
#define NUM_MESSAGES 8

char stack_sender[STACKSIZE];
char stack_receiver[STACKSIZE];

msg_t msg_array[NUM_MESSAGES];
msg_t msg_received;

void *sender_thread(void *arg) {
    msg_t msg;
    msg.content.value = 1234;
    msg.type = 1;

    while (1) {
        printf("Sender: Sending message...\n");
        msg_send_receive(&msg, &msg_received, (kernel_pid_t)arg);
        printf("Sender: Received reply: %d\n", msg_received.content.value);
        sleep(1);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    msg_t reply;
    reply.content.value = 5678;
    reply.type = 2;

    while (1) {
        printf("Receiver: Waiting for message...\n");
        msg_receive(&msg_received);
        printf("Receiver: Received message: %d\n", msg_received.content.value);
        msg_reply(&msg_received, &reply);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid, receiver_pid;

    msg_init_queue(msg_array, NUM_MESSAGES);

    receiver_pid = thread_create(stack_receiver, STACKSIZE, 1, 0, receiver_thread, NULL, "receiver");
    sender_pid = thread_create(stack_sender, STACKSIZE, 2, 0, sender_thread, (void *)receiver_pid, "sender");

    printf("Main: Threads created, sender_pid=%d, receiver_pid=%d\n", sender_pid, receiver_pid);

    while (1) {
        sleep(10);
    }

    return 0;
}
