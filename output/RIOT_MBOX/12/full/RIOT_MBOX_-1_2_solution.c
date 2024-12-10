#include "xtimer.h"
#include "ztimer.h"
#include "thread.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define INTERVAL (1000000UL) // Timer interval in microseconds

static char receiver_stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];

void *receiver(void *arg) {
    msg_init_queue(msg_queue, 8);
    evtimer_msg_event_t event;
    (void)arg;

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        event = *((evtimer_msg_event_t *)msg.content.ptr);
        printf("Event received at time: %" PRIu32 "\n", event.time);
    }
    return NULL;
}

int main(void) {
    evtimer_mbox_t mbox;
    evtimer_msg_event_t event;
    kernel_pid_t receiver_pid;

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_MAIN - 1,
                                 THREAD_CREATE_STACKTEST,
                                 receiver, NULL, "receiver");

    evtimer_mbox_init(&mbox);
    evtimer_mbox_add_event(&mbox, &event, receiver_pid, INTERVAL);

    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}