#include "net/sock/util.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "unistd.h"
#include "msg.h"

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval
#define LED_PIN GPIO_PIN(0,2) // Example LED pin, you need to set according to your board

static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED on each timer interrupt
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED_PIN as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up a recurrent timer with TIMER_INTERVAL
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);

    // Main loop to keep program running
    while (1) {
        // Just sleep to prevent the program from exiting
        xtimer_sleep(1);
    }

    return 0;
}