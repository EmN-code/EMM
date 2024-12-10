#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Assuming GPIO 2, adjust per your ESP32 board specifics

void toggle_led(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error to initialize GPIO\n");
        return 1;
    }
    
    // Timer variable
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        toggle_led(NULL);
        // Wait for one second
        xtimer_periodic_wakeup(&last_wakeup, 1000000);
    }
    return 0;
}