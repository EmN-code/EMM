
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <evtimer_mbox.h>
#include <ztimer.h>
#include <timex.h>
#include <evtimer_msg.h>

#define LED_PIN GPIO_PIN(PORT_B, 5)
#define TIMER_INTERVAL (1 * US_PER_SEC)

static void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the xtimer
    xtimer_t timer;
    xtimer_init(&timer);

    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
