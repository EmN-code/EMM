#include <timex.h>
#include <timer.h>
#include <periph/gpio.h>

// GPIO pin to which the LED is connected
#define LED_PIN GPIO_PIN(0, 2)

// Timer device
static tim_t timer_dev = TIMER_DEV(0);

// Callback function for the timer
void timer_cb(void *arg)
{
    (void)arg;
    // Toggle the LED state 
    if (gpio_read(LED_PIN) == 0) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void)
{
    // Initialize the GPIO for the LED
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Set up the timer
    uint32_t frequency = 1; // 1 Hz frequency for 1 second interval
    timer_init(timer_dev, frequency, timer_cb, NULL);
    
    // Start the timer
    timer_start(timer_dev);

    // Keep main thread running
    while (1) {
        // Implement low-power waits or other functionality if necessary
    }

    return 0;
}