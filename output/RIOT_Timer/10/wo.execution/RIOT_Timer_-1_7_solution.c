#include <xtimer.h>  // include xtimer for timer functionalities
#include <periph/gpio.h>  // include periph/gpio for GPIO access

#define LED_PIN GPIO_PIN(PORT_A, 5) // Assuming LED is connected on PORT_A pin 5

timer_t timer;

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO as output
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Setup a periodic timer
    ztimer_t periodic_timer;
    periodic_timer.callback = timer_callback;
    periodic_timer.arg = NULL;
    
    // Set a timer to trigger every second
    xtimer_ticks32_t interval = xtimer_usec_from_ticks(1000000U);
    xtimer_set(&periodic_timer, interval);
    
    while (1) {
        // Do nothing, everything is handled by the timer interrupt
    }

    return 0;
}