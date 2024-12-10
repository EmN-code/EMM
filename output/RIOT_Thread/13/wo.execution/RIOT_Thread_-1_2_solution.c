#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "msg.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define SENDER_PRIO 1
#define RECEIVER_PRIO 2
#define STACKSIZE_THREAD (THREAD_STACKSIZE_DEFAULT)

static char sender_stack[STACKSIZE_THREAD];
static char receiver_stack[STACKSIZE_THREAD];

msg_t msg_queue[8];

void *sender(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);

    msg_t msg;
    msg.type = 0x1234;
    msg.content.value = 42;  // Integer message content

    while (1) {
        xtimer_sleep(1);
        printf("Sending message with value: %d\n", msg.content.value);
        msg_send(&msg, thread_getpid());
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg)) {
            printf("Received message with value: %d\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    printf("Starting inter-thread communication example\n");

    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0,
                  receiver, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0,
                  sender, NULL, "sender");

    return 0;
}