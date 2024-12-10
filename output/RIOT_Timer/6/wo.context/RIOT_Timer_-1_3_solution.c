#include <ztimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 0)  // Use the correct port macro

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize LED_PIN as output
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize timer
    ztimer_set(ZTIMER_MSEC, &timer, 1000, timer_callback, NULL);
    
    // Main loop
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }
    return 0;
}