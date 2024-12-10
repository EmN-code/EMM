
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>

#define QUEUE_SIZE 8
#define INTERVAL_MS 1000000

msg_t msg_queue[QUEUE_SIZE];
mbox_t mbox;

void sender_thread(void) {
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(INTERVAL_MS / US_PER_SEC);
        msg_send(&msg, mbox_pid(&mbox));
    }
}

void receiver_thread(void) {
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
}

int main(void) {
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
