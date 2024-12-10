#include <timex.h>
#include <periph/gpio.h>
#include <xtimer.h>

#define LED_PIN GPIO_PIN(0, 2)  // Substitute your specific LED pin configuration here

// Timer interval of 1 second (1000000 microseconds)
#define TIMER_INTERVAL (1000000U)

// LED State
static int led_state = 0;

// Timer callback function
void timer_cb(void *arg) {
    // Toggle the LED state
    led_state = !led_state;
    gpio_write(LED_PIN, led_state);
    // Reset the timer
    xtimer_sleep(1);
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Configure the initial state of the LED to OFF
    gpio_write(LED_PIN, 0);

    // Start the periodic timer
    while (1) {
        timer_cb(NULL);
    }

    return 0;
}