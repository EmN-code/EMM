
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

void event_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Dummy event value
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    evtimer_msg_t evtimer;
    evtimer_msg_init_mbox(&evtimer, &mbox);

    evtimer_msg_add_msg(&evtimer, timex_set(1, 0), event_callback, NULL);

    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, 1) == 1) {
            printf("Received event with value: %u\n", (unsigned int)msg.content.value);
        }
    }

    return 0;
}
