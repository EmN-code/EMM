
#include <stdio.h>
#include <evtimer_mbox.h>
#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>

#define QUEUE_SIZE 16
#define INTERVAL_MS 1000

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void event_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = xtimer_now_usec();
    mbox_put(&mbox, &msg);
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init_mbox(&evtimer);

    evtimer_mbox_event_t event;
    event.event.callback = event_callback;
    event.event.arg = NULL;
    event.mbox = &mbox;

    xtimer_ticks32_t interval = xtimer_ticks_from_usec(INTERVAL_MS * 1000);
    evtimer_add_mbox(&evtimer, &event, &mbox);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received event at: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
