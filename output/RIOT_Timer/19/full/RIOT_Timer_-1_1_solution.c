#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 0) // Customize this per your board specifications

// Timer interval (1 second in microseconds)
#define INTERVAL (1000000U)

// Timer callback function
timer_cb_t toggle_led(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    return NULL;
}

int main(void) {
    // Initialize GPIO for LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing LED GPIO\n");
        return -1;
    }

    // Initialize timer
    xtimer_t timer;
    timer.callback = toggle_led;
    timer.arg = NULL;

    // Start timer with periodic schedule
    xtimer_set(&timer, INTERVAL);

    // Keep the main thread from exiting
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}