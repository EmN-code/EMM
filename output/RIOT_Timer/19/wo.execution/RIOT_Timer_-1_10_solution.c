#include <ztimer.h>
#include <periph/gpio.h>
#include <stdio.h>

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Adjust based on the actual board specifications

void timer_callback(void *arg) {
    (void)arg;  // Argument is unused
    gpio_toggle(LED_PIN);  // Toggle the LED state
}

int main(void) {
    // Initialize the GPIO pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO pin\n");
        return 1;
    }

    // Initialize a ztimer with 1 second interval
    ztimer_t timer;
    ztimer_set_message(&timer, ZTIMER_SEC, 1, timer_callback, NULL);

    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);  // Sleep for 1 second to reduce CPU usage
    }

    return 0;
}