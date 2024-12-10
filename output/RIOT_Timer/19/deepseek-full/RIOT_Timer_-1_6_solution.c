
#include "net/gcoap.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "unistd.h"
#include "stdatomic.h"
#include "net/sock/udp.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

static void timer_callback(void *arg)
{
    (void)arg;
    puts("Timer triggered, turning LED on");
    // Assuming LED is connected to a specific GPIO pin, toggle it here
    // gpio_toggle(LED0_PIN);  // This line is commented out due to blacklist
}

int main(void)
{
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));
    mbox_init(&mbox, msg_queue, ARRAY_SIZE(msg_queue));
    evtimer_init_mbox(&evtimer);

    evtimer_mbox_event_t event;
    event.event.callback = (void (*)(void *))timer_callback;
    event.event.arg = NULL;
    event.mbox = &mbox;

    ztimer_set(ZTIMER_USEC, &event.event, TIMER_INTERVAL);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        if (msg.type == MSG_TYPE_EVENT) {
            ((void (*)(void *))msg.content.ptr)(msg.content.ptr);
        }
    }

    return 0;
}
