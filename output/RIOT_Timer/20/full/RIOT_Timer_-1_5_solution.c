#include <xtimer.h>  
#include <stdlib.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)  // 1 second interval
#define LED_PIN GPIO_PIN(0,0)  // Example pin, ensure to select the correct one for your setup

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    puts("LED toggled");
}

int main(void) {
    /* Initialize the GPIO as output */
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: Unable to initialize GPIO\n");
        return EXIT_FAILURE;
    }

    /* Create a timer that calls the callback every second */
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while(1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        timer_callback(NULL);
    }

    return EXIT_SUCCESS;
}