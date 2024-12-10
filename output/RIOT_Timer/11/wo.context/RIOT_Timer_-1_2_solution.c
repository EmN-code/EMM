#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define INTERVAL (1U * US_PER_SEC) // 1 second interval
#define LED_PIN GPIO_PIN(PORT_B, 5) // Assuming LED is connected to port B, pin 5

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the GPIO pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: Unable to initialize LED pin\n");
        return 1;
    }

    // Initialize the timer
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Loop to keep the program running
    while (1) {
        // Block the thread until 1 second has passed
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        timer_callback(NULL);
    }

    return 0;
}