#include <ztimer/ztimer.h>
#include <periph/gpio.h>

// LED pin configuration
#define LED_PIN GPIO_PIN(PORT_A, 2)

// Timer interval in microseconds (1 second)
#define TIMER_INTERVAL (1U * US_PER_SEC)

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED pin
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer for periodic triggering every second
    ztimer_set_periodic(ZTIMER_MSEC, &timer, TIMER_INTERVAL);

    // Enter an infinite loop to keep the program running
    while (1) {
        // You can add other tasks here if needed
        // Keeping the MCU in low-power mode can also be done here
    }

    // Code should never reach here
    return 0;
}