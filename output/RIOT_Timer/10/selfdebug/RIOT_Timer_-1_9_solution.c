#include <xtimer.h>  // Including the xtimer header for timer functionalities
#include <periph/gpio.h> // Including GPIO peripheral header for LED control

#define LED_PIN GPIO_PIN(0, 2) // Assuming LED is connected to GPIO 0, pin 2
#define TIMER_INTERVAL (1U * US_PER_SEC) // Timer intervals in microseconds

void toggle_led(void) {
    static bool led_state = false;
    if (led_state) {
        gpio_clear(LED_PIN);
    } else {
        gpio_set(LED_PIN);
    }
    led_state = !led_state;
}

int main(void) {
    // Initialize the GPIO pin for LED
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Declare and initialize a message queue for the timer
    msg_t timer_msg_queue[8];
    msg_init_queue(timer_msg_queue, 8);

    // Setup timer to trigger every second with appropriate message handling
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        // Set the timer for the next interval
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        toggle_led(); // Toggle the LED state
    }
    return 0; // Program won't actually reach here
}