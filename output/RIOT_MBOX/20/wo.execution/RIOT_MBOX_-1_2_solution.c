#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define INTERVAL 1U

static mbox_t my_mbox;
static msg_t mbox_queue[8];

static void timer_callback(void *arg)
{
    msg_t msg;
    msg.type = 0x1234;
    if (mbox_send(&my_mbox, &msg) < 0) {
        puts("Failed to send message");
    }
}

int main(void)
{
    mbox_init(&my_mbox, mbox_queue, sizeof(mbox_queue));

    ztimer_t timer;
    ztimer_init(ZTIMER_MSEC, &timer, 0);
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL * 1000);

    while (1) {
        msg_t msg;
        if (_mbox_get(&my_mbox, &msg, 1)) {
            puts("Message received");
        }
        xtimer_sleep(INTERVAL);
    }

    return 0;
}