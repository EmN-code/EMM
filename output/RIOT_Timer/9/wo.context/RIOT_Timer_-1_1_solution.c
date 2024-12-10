#include <timex.h>
#include <timer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN     GPIO_PIN(PORT_B, 0)

tim_t timer_dev = TIMER_DEV(0);

timer_cb_t timer_cb(void *arg) {
    (void)arg;
    gpio_set(LED_PIN);
    printf("LED is ON\n");
}

int main(void) {
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize timer
    uint32_t freq = 1; // 1 Hz frequency
    timer_init(timer_dev, freq, timer_cb, NULL);
    
    // Start timer
    timer_start(timer_dev);

    while (1) {
        // Main loop can perform other tasks
    }
    return 0;
}