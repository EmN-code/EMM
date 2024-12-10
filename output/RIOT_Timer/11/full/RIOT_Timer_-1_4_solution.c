#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <unistd.h>
#include <mbox.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>

#define INTERVAL (1U * US_PER_SEC)
#define LED_PIN 2  // Example pin number, change according to your configuration

static ztimer_t timer;
static bool led_is_on = false;

static void timer_callback(void *arg)
{
    (void)arg;
    led_is_on = !led_is_on; // Toggle LED state
    // gpio_toggle(LED_PIN); // Uncomment when using actual GPIO
    printf("LED is now %s\n", led_is_on ? "ON" : "OFF");
}

int main(void)
{
    // gpio_init(LED_PIN, GPIO_OUT); // Uncomment for actual GPIO initialization
    
    ztimer_set(&timer, ztimer_now(ZTIMER_USEC) + INTERVAL, timer_callback, NULL);
    printf("Timer setup complete. Toggling LED every second.\n");

    while (1) {
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
        ztimer_set(&timer, ztimer_now(ZTIMER_USEC) + INTERVAL, timer_callback, NULL);
    }

    return 0;
}