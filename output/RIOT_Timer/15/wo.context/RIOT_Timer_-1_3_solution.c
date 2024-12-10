#include <stdio.h>
#include <timex.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC) // Timer set to 1 second

// LED pin configuration
#define LED_PIN GPIO_PIN(PORT_A, 5)  // Modify based on your board's LED pin

static void timer_callback(void *arg) {
    (void)arg;
    static int led_state = 0;
    led_state = !led_state;  // Toggle LED state
    gpio_write(LED_PIN, led_state);
}

int main(void) {
    int rc;

    // Initialize the LED pin
    if ((rc = gpio_init(LED_PIN, GPIO_OUT)) < 0) {
        printf("Error initializing GPIO\n");
        return 1;
    }

    // Initialize timer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        if ((rc = xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL)) != 0) {
            printf("Timer error\n");
            continue;  // Continue the while loop if there's an error
        }
        timer_callback(NULL);  // Call the timer callback function
    }

    return 0;
}