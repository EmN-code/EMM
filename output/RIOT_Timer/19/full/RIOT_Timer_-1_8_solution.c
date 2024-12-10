#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 6)
#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval

static void _timer_callback(void *arg) {
    (void)arg;
    // Toggle LED state
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the GPIO pin as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing the GPIO pin\n");
        return 1;
    }
    
    // Initialize the xtimer
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        // Wait for the next timer interval
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        _timer_callback(NULL);
    }
    return 0;
}