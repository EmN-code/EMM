#include <stdio.h>
#include <timex.h>
#include <timer.h>
#include <gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 1)

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    
    tim_t dev = TIMER_DEV(0);
    uint32_t freq = 1 * US_PER_SEC; // 1 Hz frequency for 1 second intervals
    timer_init(dev, freq, timer_callback, NULL);
    timer_start(dev);

    // Idle loop
    while (1) {
        // do nothing, everything is handled by the timer interrupt
    }

    return 0;
}