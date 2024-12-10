#include "xtimer.h"
#include "ztimer.h"
#include "time_units.h"
#include "timex.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define INTERVAL (1U * US_PER_SEC)
#define MSG_ARRAY_SIZE (8U)

static evtimer_msg_event_t evtimer_event;
static msg_t msg_queue[MSG_ARRAY_SIZE];

static void event_handler(msg_t *msg)
{
    printf("Received event with type: %d\n", msg->type);
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_ARRAY_SIZE);

    evtimer_event.callback = event_handler;
    evtimer_event.event.msg = &evtimer_event.event;
    evtimer_event.event.msg->type = EVTIMER_MSG;

    if (evtimer_mbox_add_event(&evtimer_event.event, INTERVAL) != 0) {
        printf("Error setting event timer\n");
        return 1;
    }

    printf("Starting event loop\n");
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        event_handler(&msg);
    }

    return 0;
}