// Description:
// This program demonstrates the use of a mailbox and event timer in RIOT OS on an ESP32. We set up a message box, initialize an event timer, and send events at regular intervals.

#include <xtimer.h>
#include <evtimer.h>
#include <event/callback.h>
#include <msg.h>
#include <mbox.h>

#define INTERVAL (1000000) // 1 second interval

static mbox_t my_mbox;
static msg_t mbox_queue[8];

static evtimer_t evtimer;
static evtimer_event_t evtimer_event;

void send_event(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0xA0;
    msg.content.value = 42;
    mbox_send(&my_mbox, &msg);
    evtimer_add(&evtimer, &evtimer_event);
}

int main(void) {
    // Initialize the mailbox
    mbox_init(&my_mbox, mbox_queue, sizeof(mbox_queue) / sizeof(mbox_queue[0]));

    // Initialize event timer
    evtimer_init_msg(&evtimer, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));
    evtimer_set_msg(&evtimer_event, send_event, NULL, thread_getpid());
    evtimer_add(&evtimer, &evtimer_event);

    // Main loop
    while (1) {
        msg_t msg;
        mbox_get(&my_mbox, &msg, 1);
        if (msg.type == 0xA0) {
            printf("Received event with value: %u\n", msg.content.value);
        }
        xtimer_sleep(1);
    }

    return 0;
}