#include <stdio.h>
#include <stdlib.h>
#include <random.h>
#include <msg.h>
#include <mbox.h>
#include "thread.h"

#define MSG_QUEUE_SIZE 8
#define MBOX_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack[THREAD_STACKSIZE_DEFAULT];

static mbox_t mbox;
static msg_t mbox_queue[MBOX_SIZE];

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    random_bytes(&msg.content.value, sizeof(msg.content.value));
    while(1) {
        if (mbox_put(&mbox, &msg, KERNEL_MSG_BLOCK) != 1) {
            printf("Failed to post message to mbox\n");
        }
        else {
            printf("Message posted: %lu\n", msg.content.value);
        }
        xtimer_sleep(2);
    }
}

void receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while(1) {
        if (mbox_get(&mbox, &msg) == 1) {
            printf("Message received: %lu\n", msg.content.value);
        }
        else {
            printf("Failed to fetch message from mbox\n");
        }
    }
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, mbox_queue, MBOX_SIZE);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  sender_thread, NULL, "sender");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 2, 0,
                  receiver_thread, NULL, "receiver");
    return 0;
}