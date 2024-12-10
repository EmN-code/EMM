#include <ztimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Assuming LED is connected to PA5

void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    gpio_toggle(LED_PIN);  // Toggle the LED state
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize a periodic timer that triggers every second
    ztimer_t timer;
    ztimer_set(&timer, ZTIMER_MSEC, 1000, timer_callback, NULL);

    while (1) {
        // Sleep for 1 second, then wake up
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;  // This point will never be reached
}