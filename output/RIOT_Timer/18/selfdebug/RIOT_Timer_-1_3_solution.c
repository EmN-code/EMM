#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

// Timer interval set to 1 second (1000000 microseconds)
#define INTERVAL (1000000U)
#define LED_PIN GPIO_PIN(PORT_A, 10) // Assuming the LED is connected to pin 10 of Port A

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled!\n");
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create and start a periodic timer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        timer_callback(NULL);
    }

    return 0;
}