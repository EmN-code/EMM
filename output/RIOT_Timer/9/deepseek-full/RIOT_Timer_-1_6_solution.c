
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
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "net/sock/udp.h"
#include "mma8x5x_params.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static evtimer_msg_t evtimer;

void timer_callback(void *arg)
{
    (void)arg;
    puts("Timer triggered, turning LED on.");
    // Assuming LED is connected to a specific GPIO pin, toggle it here
    // For example, if LED is connected to GPIO pin 5:
    // gpio_toggle(GPIO_PIN(0, 5));
}

int main(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    evtimer_init_msg(&evtimer);

    // Set up the timer to trigger every second
    evtimer_add_msg(&evtimer, ztimer_now(ZTIMER_USEC) + TIMER_INTERVAL, timer_callback, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle received messages if needed
    }

    return 0;
}
