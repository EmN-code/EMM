#include "ztimer.h"
#include "xtimer.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "time_units.h"
#include "timex.h"
#include "sys/socket.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph_conf.h"
#include "kernel_defines.h"
#include "thread.h"
#include "stdlib.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define LED_PIN 2

evtimer_t evtimer;
msg_t msg_queue[8];

void handle_timer_event(void *arg)
{
    (void)arg;
    printf("Timer event triggered!\n");
    // Logic to toggle an LED
}

void timer_setup(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));
    evtimer_init_mbox(&evtimer);
    evtimer_add(&evtimer, 1000 * 1000, handle_timer_event, NULL);
}

int main(void)
{
    printf("Starting application...\n");

    // Set up timer
    timer_setup();

    while (1) {
        // Application loop here
        evtimer_do_work(&evtimer);
    }
    return 0;
}