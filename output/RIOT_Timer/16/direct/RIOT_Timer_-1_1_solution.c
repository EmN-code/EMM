#include <ztimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_GPIO_PIN GPIO_PIN(0, 2)  // Example GPIO pin for LED
#define BLINK_PERIOD (ZTIMER_SEC)    // Timer period set to 1 second

void toggle_led(void *arg) {
    (void)arg;
    static int led_state = 0;
    
    // Toggle LED state
    led_state = !led_state;
    gpio_write(LED_GPIO_PIN, led_state);
}

int main(void) {
    // Initialize the GPIO pin as output for LED
    gpio_init(LED_GPIO_PIN, GPIO_OUT);
    
    printf("Starting LED Blink Timer\n");
    
    // Initialize ztimer for periodic interrupt
    ztimer_periodic_t periodic_timer;
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, toggle_led, NULL, BLINK_PERIOD);
    
    // Main loop can handle other tasks if necessary or just sleep
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1); // Sleep to save CPU power when idle
    }
    
    return 0;
}