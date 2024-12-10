#include <thread.h>
#include <timex.h>
#include <timer.h>
#include <gpio.h>

#define LED_PIN GPIO_PIN(0, 2)  // Assuming GPIO PIN 0, line 2 for LED

tim_t timer_device = TIMER_DEV(0);

void toggle_led(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Set up the timer
    if (timer_init(timer_device, 1000000, toggle_led, NULL) != 0) {
        printf("Timer initialization failed\n");
        return 1;
    }

    timer_start(timer_device);

    // Keep the main thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}