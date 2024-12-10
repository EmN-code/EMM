
#include <stdlib.h>
#include <net/emcute.h>
#include <msg.h>
#include <periph/gpio.h>
#include <time_units.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <net/gcoap.h>

#define LED_PIN GPIO_PIN(0, 13)  // Example GPIO pin for LED on ESP32

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    xtimer_t timer;
    xtimer_init(&timer);

    // Set the timer to trigger every second
    xtimer_set_periodic(&timer, 1 * US_PER_SEC, timer_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
