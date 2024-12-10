
#include "xtimer.h"
#include "msg.h"
#include "periph/gpio.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static volatile int event_triggered = 0;

static void periodic_callback(void *arg) {
    (void)arg;
    event_triggered = 1;
}

int main(void) {
    xtimer_t timer;
    msg_t msg;

    // Initialize the timer
    xtimer_init(&timer);

    // Set up the periodic callback
    xtimer_set_periodic(&timer, INTERVAL_MS, periodic_callback, NULL);

    while (1) {
        if (event_triggered) {
            event_triggered = 0;  // Reset the event flag
            // Perform your periodic task here
            // For example, toggle a GPIO pin
            gpio_toggle(GPIO_PIN(0, 1));  // Toggle GPIO 0.1
        }
        // Yield to other threads
        msg_receive(&msg);
    }

    return 0;
}
