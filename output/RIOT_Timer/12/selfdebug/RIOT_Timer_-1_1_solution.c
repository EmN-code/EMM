#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 0)  // Assuming the LED is connected to pin A0
#define INTERVAL (1U * US_PER_SEC)  // Timer interval in microseconds (1 second)

void led_toggle(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up a timer that triggers every second
    while (1) {
        led_toggle();
        xtimer_usleep(INTERVAL);
    }
    return 0;
}