#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"

#define LED_PIN     GPIO_PIN(0, 2)
#define INTERVAL    (1U * US_PER_SEC)  // 1 second interval

void timer_callback(void *arg) {
    (void)arg;
    gpio_write(LED_PIN, 1);  // Turn on LED
    xtimer_usleep(100000);   // Keep LED on for 100ms
    gpio_write(LED_PIN, 0);  // Turn off LED
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Call a function that simulates an API interaction
        int rc = simulated_api_call();
        if (rc != 0) {
            printf("API call failed, retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        }
        
        // Timer logic for LED
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        timer_callback(NULL);
    }
    return 0;
}

int simulated_api_call(void) {
    // Simulate successful call
    return 0;
}