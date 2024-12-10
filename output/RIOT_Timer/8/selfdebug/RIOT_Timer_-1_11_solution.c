#include <xtimer.h>  // For using the xtimer API
#include <timex.h>   // For time intervals
#include <stdio.h>   // For printf debugging
#include <periph/gpio.h>  // For GPIO control

#define TIMER_INTERVAL (1 * US_PER_SEC)
#define LED_PIN GPIO_PIN(0, 2) // Using GPIO0, pin 2 for the LED, adjust as per your board

static void timer_callback(void *arg) {
    (void)arg; // We don't use the argument here
    static int led_state = 0;
    led_state = !led_state;  // Toggle LED state
    gpio_write(LED_PIN, led_state);
    printf("LED is %s\n", led_state ? "ON" : "OFF");
}

int main(void) {
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO\n");
        return 1;
    }
    
    // Start the timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    
    printf("Starting timer\n");
    xtimer_set(&timer, TIMER_INTERVAL);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }
    
    return 0;
}