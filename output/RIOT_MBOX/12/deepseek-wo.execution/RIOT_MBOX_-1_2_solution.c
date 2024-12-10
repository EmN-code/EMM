
#include "ztimer.h"
#include "xtimer.h"
#include "msg.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "thread.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static evtimer_mbox_t mbox;

void event_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Example event value
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_mbox_init(&mbox, event_callback, NULL);

    // Schedule an event every 1 second
    evtimer_mbox_add_msg(&mbox, ztimer_now(ZTIMER_MSEC) + ZTIMER_SEC(1), NULL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received event with value: %d\n", msg.content.value);
    }

    return 0;
}
