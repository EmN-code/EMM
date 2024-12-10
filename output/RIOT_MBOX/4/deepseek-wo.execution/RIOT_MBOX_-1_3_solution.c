
#include <stdio.h>
#include <evtimer_msg.h>
#include <ztimer.h>
#include <msg.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <xtimer.h>
#include <mbox.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void send_event(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Example event value
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init_mbox(&mbox);

    // Add an event to be sent every 1 second
    evtimer_add_msg(ztimer_now(ZTIMER_MSEC) + ZTIMER_TICKS(1000), send_event, NULL);

    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, 1) == 1) {
            printf("Received event with value: %d\n", msg.content.value);
        }
    }

    return 0;
}
