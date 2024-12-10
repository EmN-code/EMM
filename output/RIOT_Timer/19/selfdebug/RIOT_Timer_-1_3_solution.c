#include <xtimer.h>
#include <periph/gpio.h>

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval
#define LED_PIN GPIO_PIN(PORT_A, 10)  // Assuming LED is connected to pin PA10

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // toggle LED state
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        timer_callback(NULL);
    }
    return 0;
}