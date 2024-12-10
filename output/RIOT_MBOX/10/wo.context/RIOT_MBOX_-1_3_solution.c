#include <xtimer.h>
#include <kernel_defines.h>
#include <event/callback.h>
#include <evtimer.h>
#include <core_msg.h>
#include <nimble/ble.h>
#include <sock_types.h>
#include <periph/i2c.h>
#include <host/ble_gap.h>

#define INTERVAL (1U * US_PER_SEC)
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;
static evtimer_event_t event;

static void _event_handler(evtimer_event_t *event)
{
    msg_t msg;
    msg.content.value = 42; // Example message content
    mbox_send(&mbox, &msg); // Corrected function call
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    evtimer_init(&evtimer, NULL, 0);
    evtimer_set(&event, INTERVAL, _event_handler, NULL);
    evtimer_add(&evtimer, &event);

    evtimer_run(&evtimer);
    return 0;
}