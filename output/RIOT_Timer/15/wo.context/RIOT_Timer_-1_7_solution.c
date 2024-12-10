#include "xtimer.h"
#include "thread.h"
#include "board.h"
#include "periph_gpio.h"

#define LED_PIN    GPIO_PIN(PORT_A, 5) // Assuming an LED is connected to pin PORT_A, 5

void *led_handler(void *arg) {
    (void)arg;
    // Initialize GPIO pin for output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: unable to initialize GPIO pin as output.\n");
        return NULL;
    }
    
    while (1) {
        // Turn on the LED
        gpio_set(LED_PIN);
        printf("LED On\n");

        // Wait for one second
        xtimer_sleep(1);

        // Turn off the LED
        gpio_clear(LED_PIN);
        printf("LED Off\n");

        // Wait for one second
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    // Start the LED handler thread
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, led_handler,
                  NULL, "led");

    // Main thread will sleep forever
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}