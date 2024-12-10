
#include "evtimer_msg.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "xtimer.h"
#include "mbox.h"

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
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    evtimer_init_msg(&mbox);

    ztimer_t timer;
    timer.callback = send_event;
    timer.arg = NULL;
    timer.interval = ZTIMER_SEC(1); // Set interval to 1 second

    evtimer_add_msg(&timer);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received event with value: %d\n", (int)msg.content.value);
    }

    return 0;
}
