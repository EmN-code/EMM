#include "msg.h"
#include "thread.h"
#include "stdio.h"
#include "xtimer.h"

#define SENDER_PRIO       (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO     (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static msg_t msg_queue[8];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;

    msg.type = 1;
    msg.content.value = 42;
    while (1) {
        if (msg_send(&msg, thread_get(RECEIVER_PRIO)) < 1) {
            puts("Failed to send message");
        } else {
            puts("Message sent");
        }
        xtimer_sleep(1);
    }

    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    thread_create(sender_stack, sizeof(sender_stack),
                  SENDER_PRIO, THREAD_CREATE_STACKTEST,
                  sender, NULL, "sender");

    thread_create(receiver_stack, sizeof(receiver_stack),
                  RECEIVER_PRIO, THREAD_CREATE_STACKTEST,
                  receiver, NULL, "receiver");

    return 0;
}
