#include <ztimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Assuming LED is connected to pin 0,2
#define INTERVAL (1U * US_PER_SEC)

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Setup LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    ztimer_t timer;
    ztimer_set_msg_timeout(ztimer_desc(ZTIMER_USEC), &timer, INTERVAL, timer_callback, NULL);

    // Start the timer
    ztimer_start(&timer);

    while(1) {
        // Map here any real-time tasks or handle other applications
    }
    return 0;
}