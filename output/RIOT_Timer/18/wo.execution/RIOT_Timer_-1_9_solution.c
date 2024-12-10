#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "string.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"

#define LED_PIN GPIO_PIN(0, 23) // Define GPIO pin number you are using
#define INTERVAL (1U * US_PER_SEC) // Timer interval of 1 second

static ztimer_t timer;
static msg_t msg_queue[8];
static msg_t msg;

static void timer_callback(void *arg) {
    (void)arg;
    msg.content.ptr = NULL;
    msg_try_send(&msg, thread_getpid());
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, 8);
    
    // Initialize GPIO
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the timer
    ztimer_alarm_init(ZTIMER_USEC, &timer, timer_callback, NULL, INTERVAL);
    ztimer_alarm_start(ZTIMER_USEC, &timer);

    while (1) {
        // Wait for the next timer event
        msg_receive(&msg);
        // Toggle the LED
        gpio_toggle(LED_PIN);
    }

    return 0;
}