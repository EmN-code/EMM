#include <string.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <event/thread.h>
#include <timex.h>
#include <ztimer.h>
#include <msg.h>
#include <mutex.h>
#include <unistd.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 3
#define THREAD_PRIORITY_HIGH 2
#define THREAD_PRIORITY_LOW 4
#define QUEUE_SIZE 8

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];
static msg_t sender_queue[QUEUE_SIZE];
static msg_t receiver_queue[QUEUE_SIZE];

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

void *sender(void *arg) {
    (void)arg;
    msg_t message;
    message.type = 0x5678;

    while (1) {
        msg_send(&message, receiver_pid);
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t message;

    while (1) {
        msg_receive(&message);
        printf("Received message of type: %u\n", (unsigned)message.type);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(sender_queue, QUEUE_SIZE);
    msg_init_queue(receiver_queue, QUEUE_SIZE);

    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               THREAD_PRIORITY_LOW, 0, sender,
                               NULL, "sender");
                                
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_HIGH, 0, receiver,
                                 NULL, "receiver");
    
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}