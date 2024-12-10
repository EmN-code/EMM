
#include <stdlib.h>
#include <msg.h>
#include <periph/gpio.h>
#include <time_units.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define LED_PIN GPIO_PIN(0, 16)  // Example GPIO pin for LED on ESP32
#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static xtimer_t timer;
static volatile bool led_state = false;

void timer_callback(void) {
    gpio_toggle(LED_PIN);  // Toggle the LED state
    led_state = !led_state;
}

int main(void) {
    // Initialize the GPIO pin for the LED
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Failed to initialize GPIO\n");
        return 1;
    }

    // Set up the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback);

    // Main loop to keep the program running
    while (1) {
        // The timer will handle the LED toggling
        xtimer_usleep(TIMER_INTERVAL);  // Sleep for the timer interval
    }

    return 0;
}
