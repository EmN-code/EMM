#include "ztimer.h"
#include "time_units.h"
#include "xtimer.h"
#include "timex.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "evtimer.h"

#define NUM_MESSAGES 4
#define INTERVAL (1U * US_PER_SEC)  // 1 second interval

evtimer_t evtimer;
msg_t msg_queue[NUM_MESSAGES];

static void cb(void *arg)
{
    (void)arg;
    puts("Event triggered!");
}

int main(void)
{
    puts("RIOT event loop example with mbox");

    msg_init_queue(msg_queue, NUM_MESSAGES);

    evtimer_mbox_t event;
    evtimer_init_mbox(&evtimer);

    event.event.callback = cb;
    event.event.period = INTERVAL;
    evtimer_add_mbox(&evtimer, &event, &msg_queue);

    msg_t m;
    while (1) {
        msg_receive(&m);
        // Process message
        printf("Received message from PID: %d\n", m.sender_pid);
    }

    return 0;
}