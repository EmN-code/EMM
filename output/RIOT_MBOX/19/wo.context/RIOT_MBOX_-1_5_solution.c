#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "mutex.h"
#include "mbox.h"

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define MBOX_SIZE (4U)

static mbox_t mbox;
static msg_t mbox_buffer[MBOX_SIZE];
static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        printf("Sending message with value: %ld\n", (long)msg.content.value);
        mbox_put(&mbox, &msg);
        xtimer_sleep(2);
    }

    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %ld\n", (long)msg.content.value);
    }

    return NULL;
}

int main(void) {
    mbox_init(&mbox, mbox_buffer, MBOX_SIZE);
    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO,
                  THREAD_CREATE_STACKTEST, sender, NULL, "sender thread");
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO,
                  THREAD_CREATE_STACKTEST, receiver, NULL, "receiver thread");

    return 0;
}