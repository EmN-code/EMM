
#include <stdio.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <mma8x5x_regs.h>
#include <paho_mqtt.h>
#include <timex.h>
#include <ztimer.h>
#include <msg.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <mbox.h>
#include <xtimer.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void _evtimer_msg_handler(evtimer_event_t *event)
{
    msg_t msg;
    msg.content.value = (uint32_t)event;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init_msg(&evtimer);

    evtimer_mbox_event_t event;
    evtimer_mbox_init(&event, &mbox);

    xtimer_t timer;
    timer.target = timer.long_target = xtimer_now();
    timer.callback = (xtimer_callback_t)evtimer_add_mbox;
    timer.arg = &event;

    while (1) {
        xtimer_set(&timer, 1000000); // 1 second interval
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
