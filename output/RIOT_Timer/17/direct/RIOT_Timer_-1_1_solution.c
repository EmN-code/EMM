#include <ztimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 5)
#define TIMER_INTERVAL (1LU * US_PER_SEC)  // 1 second interval

static ztimer_t timer;

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Set up the timer
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL, timer_callback, NULL);

    // Main loop does nothing, the timer interrupts handle everything
    while(1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }

    return 0;
}