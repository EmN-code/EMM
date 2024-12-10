#include <xtimer.h>
#include <periph/gpio.h>

// Define the LED pin according to your ESP32 setup
#define LED_PIN GPIO_PIN(PORT_B, 2) // Example port and pin

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Loop to trigger callback every second
    while(1) {
        xtimer_sleep(1);
        timer_callback(NULL);
    }
    return 0;
}