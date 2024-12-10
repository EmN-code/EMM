#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_A, 2)  // Replace with your actual GPIO pin
#define INTERVAL 1  // Timer interval in seconds

void trigger_led(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize the timer
    while (1) {
        trigger_led();
        xtimer_sleep(INTERVAL);
    }
    
    return 0;
}