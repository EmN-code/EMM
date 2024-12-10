#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Define the pin where the LED is connected
#define INTERVAL (1U * US_PER_SEC)   // Timer interval in microseconds

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED state
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);  // Initialize the GPIO pin as output

    // Define and initialize a timer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        timer_callback(NULL);  // Manually trigger the callback
    }
    return 0;
}