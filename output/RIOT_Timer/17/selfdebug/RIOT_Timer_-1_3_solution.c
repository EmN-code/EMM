#include <ztimer.h>
#include <timex.h>
#include <periph/gpio.h>
#include <stdio.h>

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Example GPIO port and pin for LED

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO\n");
        return 1;
    }
    
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        timer_callback(NULL);
    }
    return 0;
}