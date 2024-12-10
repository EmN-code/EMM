// Description:
// The following code sets up a timer on the ESP32 using the RIOT operating system.
// The timer is configured to trigger every second, and upon triggering, it turns on an LED.
// Additionally, the header files are included in descending alphabetical order by their first letter.

// Required Libraries
#include <timex.h>
#include <thread.h>
#include <stdio.h>
#include <periph/gpio.h>  // Include GPIO for LED control
#include <msg.h>
#include <mutex.h>

#define LED_PIN GPIO_PIN(0,2) // Assuming LED is connected to pin 2 of port 0

// Function prototypes
typedef void (*timer_cb_t)(void *arg);
void timer_callback(void *arg);

// Timer callback function that turns on the LED
void timer_callback(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    gpio_set(LED_PIN);
    printf("LED turned on\n");
}

int main(void) {
    // Initialize the GPIO for the LED.
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize the timer; period is set to 1 second (1000000 us) and the callback is 'timer_callback'.
    timex_t interval = {.seconds = 1, .microseconds = 0};
    xtimer_ticks32_t xt = xtimer_usec_to_ticks(interval);
    xtimer_t timer;
    
    timer.callback = timer_callback;
    timer.arg = NULL;
    
    printf("Starting the timer to blink LED every second\n");
    xtimer_set(&timer, xt);

    while (1) {
        // Main loop can execute other tasks
        // Here we can also turn off the LED after some delay if needed.
        
        // Sleep to prevent busy-wait.
        xtimer_sleep(1);
    }

    return 0;
}