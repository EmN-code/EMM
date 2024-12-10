#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define SENDER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define HIGH_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define LOW_PRIORITY (THREAD_PRIORITY_MAIN + 1)

msg_t queue[8];

static char sender_stack[SENDER_STACKSIZE];
static char receiver_stack[RECEIVER_STACKSIZE];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;

    msg.type = 1;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(1); // sending message every second
        printf("Sender: Sending message with value: %ld\n", (unsigned long)msg.content.value);
        msg_send(&msg, thread_getpid());
    }

    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value: %ld\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                            HIGH_PRIORITY, 0, sender, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                              LOW_PRIORITY, 0, receiver, NULL, "receiver");

    while (1) {
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }

    return 0;
}