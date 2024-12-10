#include <xtimer.h>
#include <stdio.h>
#include <msg.h>
#include <kernel.h>

#define MAIN_QUEUE_SIZE     (8)
#define MSG_TYPE            (0x1234)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static mbox_t mbox;
static msg_t mbox_buffer[4];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = MSG_TYPE;

    while (1) {
        for (uint32_t i = 0; i < 10; i++) {
            msg.content.value = i;
            if (mbox_try_put(&mbox, &msg) == 1) {
                printf("Sent: %lu\n", msg.content.value);
            }
            xtimer_sleep(1);
        }
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (_mbox_get(&mbox, &msg, 1) == 1) {
            printf("Received: %lu\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    printf("Starting message box example\n");

    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    mbox_init(&mbox, mbox_buffer, sizeof(mbox_buffer)/sizeof(mbox_buffer[0]));

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}