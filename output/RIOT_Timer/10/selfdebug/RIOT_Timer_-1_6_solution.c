#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)
#define LED_PIN        GPIO_PIN(0, 2) // Use appropriate GPIO port and pin for your LED

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED state toggled\n");
}

int main(void) {
    // Initialize the GPIO as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error to initialize GPIO\n");
        return -1;
    }
    
    // Initialize a timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    
    printf("Starting timer interrupt every second\n");
    while (1) {
        xtimer_set(&timer, TIMER_INTERVAL);
        xtimer_sleep(1);
    }
    return 0;
}