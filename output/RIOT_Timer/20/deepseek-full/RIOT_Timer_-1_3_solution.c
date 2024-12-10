
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

static void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Dummy value to indicate timer event
    msg_send(&msg, mbox_fd(&mbox));
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);
    evtimer_init(&evtimer, timer_callback, NULL);

    evtimer_msg_event_t event;
    event.event.offset = 1 * US_PER_SEC; // 1 second
    event.event.callback = timer_callback;
    event.mbox = &mbox;

    evtimer_add(&evtimer, (evtimer_event_t *)&event);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle the message, e.g., toggle an LED (not using gpio_toggle here)
    }

    return 0;
}
