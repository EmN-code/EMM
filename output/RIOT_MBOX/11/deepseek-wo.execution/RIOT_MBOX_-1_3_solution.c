
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "mbox.h"

#define QUEUE_SIZE 8
#define STACK_SIZE 512
#define PRIORITY 5

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        xtimer_usleep(1000000); // Sleep for 1 second
    }
}

void receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    char stack1[STACK_SIZE];
    char stack2[STACK_SIZE];

    thread_create(stack1, STACK_SIZE, PRIORITY, 0, sender_thread, NULL, "sender");
    thread_create(stack2, STACK_SIZE, PRIORITY, 0, receiver_thread, NULL, "receiver");

    return 0;
}
