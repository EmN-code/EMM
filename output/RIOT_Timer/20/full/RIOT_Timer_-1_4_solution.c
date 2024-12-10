#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 15)  // Adjust the port and pin numbers according to your board

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval

void toggle_led(void) {
    static int state = 0;
    state = !state;
    if (state) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
    printf("LED toggled to state: %d\n", state);
}

int main(void) {
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error: Unable to initialize GPIO\n");
        return EXIT_FAILURE;
    }
    
    printf("Timer setup to toggle LED every second\n");
    
    xtimer_ticks32_t last_wakeup = xtimer_now();
    
    while (1) {
        toggle_led();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return EXIT_SUCCESS;  // Though we will never reach this point
}