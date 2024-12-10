
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void) {
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        msg_send(&msg, mbox_pid);
        xtimer_sleep(INTERVAL_MS / MS_IN_USEC);
    }
}

void receiver_thread(void) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
}

int main(void) {
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender");

    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver");

    return 0;
}
