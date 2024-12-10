#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Define the GPIO pin for the LED
#define INTERVAL (1U * US_PER_SEC)  // 1-second interval

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void) {
    printf("Start LED toggle example\n");
    // Initialize the GPIO pin for output
    if(gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: could not initialize LED pin\n");
        return 1;
    }

    // Initialize the xtimer and set the callback
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        timer_callback(NULL);
    }

    return 0;
}