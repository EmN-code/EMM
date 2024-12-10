
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mbox.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void) {
    msg_t msg;
    int count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        printf("Sent message: %d\n", msg.content.value);
        sleep(1);
    }
}

void receiver_thread(void) {
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received message: %d\n", msg.content.value);
    }
}

int main(void) {
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Create sender thread
    if (thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, (thread_task_func_t)sender_thread, NULL, "sender") < 0) {
        printf("Failed to create sender thread\n");
        return 1;
    }

    // Create receiver thread
    if (thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, (thread_task_func_t)receiver_thread, NULL, "receiver") < 0) {
        printf("Failed to create receiver thread\n");
        return 1;
    }

    return 0;
}
