#include <timex.h>
#include <timers/timer.h>
#include <thread.h>
#include <xtimer.h>

#define LED_PIN 2 // Assuming the LED is connected to GPIO 2

// Timer callback function
static void timer_callback(void *arg)
{
    (void)arg;
    // Toggle the LED state
    if (gpio_read(LED_PIN)) {
        gpio_clear(LED_PIN);
    }
    else {
        gpio_set(LED_PIN);
    }
}

int main(void)
{
    // Initialize the LED pin as an output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create and initialize the timer
    tim_t timer_dev = TIMER_DEV(0); // Using timer 0
    timer_init(timer_dev, 1, timer_callback, NULL);
    
    // Start the timer with 1 second interval
    timer_set_absolute(timer_dev, 1*XTIMER_HZ);

    // Keep the thread alive
    while (1) {
        thread_yield();
    }
    return 0;
}