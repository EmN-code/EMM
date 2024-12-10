#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define TIMER_INTERVAL (1000000U) // 1 second
#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static ztimer_t timer;
static mbox_t mbox;
static evtimer_msg_event_t evtimer_event;
static msg_t msg;

static void evtimer_callback(void *arg) {
    (void)arg;
    printf("Event triggered!\n");
}

int main(void) {
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    evtimer_init_msg(&evtimer_event, &mbox, evtimer_callback, NULL);
    ztimer_set_msg(ZTIMER_USEC, &timer, TIMER_INTERVAL, &msg, thread_getpid());

    while (1) {
        msg_receive(&msg);
        evtimer_msg_event_trigger(&evtimer_event);
    }

    return 0;
}