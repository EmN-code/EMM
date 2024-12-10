#include "xtimer.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "stdbool.h"

#define MAIN_QUEUE_SIZE     (8)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

static mbox_t mbox;
static msg_t mbox_queue[4];

void sender_thread(void) {
    msg_t msg;
    msg.content.value = 100;
    while (1) {
        mbox_send(&mbox, &msg);
        xtimer_sleep(2);
    }
}

void receiver_thread(void) {
    msg_t msg;
    while (1) {
        mbox_receive(&mbox, &msg);
        printf("Message received with value: %u\n", (unsigned)msg.content.value);
    }
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_init(&mbox, mbox_queue, 4);

    char sender_stack[THREAD_STACKSIZE_MAIN];
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1,
                  0, sender_thread, NULL, "sender");

    char receiver_stack[THREAD_STACKSIZE_MAIN];
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1,
                  0, receiver_thread, NULL, "receiver");

    return 0;
}