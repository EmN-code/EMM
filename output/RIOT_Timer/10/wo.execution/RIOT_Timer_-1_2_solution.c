#include <ztimer.h>   // Ztimer is used for more complex timers but can replace xtimer in case of deprecation
#include <xtimer.h>   // For delay functions
#include <periph/gpio.h>   // For handling GPIO pins

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Assuming the LED is connected to the PA5 pin

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle LED state
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the xtimer
    ztimer_t timer;
    ztimer_init(ZTIMER_MSEC, &timer, timer_callback, NULL, 1000);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);  // Sleep for 1 second
    }
    return 0;
}