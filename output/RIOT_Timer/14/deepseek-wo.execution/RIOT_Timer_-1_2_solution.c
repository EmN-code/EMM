
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <evtimer_mbox.h>
#include <ztimer.h>
#include <timex.h>
#include <evtimer_msg.h>

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO0, pin 13 for LED
#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void _timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void)
{
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the timer
    xtimer_t timer;
    xtimer_set(&timer, TIMER_INTERVAL, _timer_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
