
#include <kernel_defines.h>
#include <thread.h>
#include <mbox.h>
#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE 16
#define SEND_INTERVAL 1000000  // 1 second in microseconds

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        mbox_put(&mbox, &msg);
        xtimer_usleep(SEND_INTERVAL);
    }
}

void receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %u\n", (unsigned int)msg.content.value);
    }
}

int main(void) {
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");

    return 0;
}
