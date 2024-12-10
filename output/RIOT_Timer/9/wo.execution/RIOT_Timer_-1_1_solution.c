#include "timer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_B, 0)
#define TIMER_DEV TIMER_DEV(0)

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggles the state of the LED
}

int main(void) {
    int freq = 1;  // 1 Hz toggle frequency
    gpio_init(LED_PIN, GPIO_OUT);
    timer_init(TIMER_DEV, freq, timer_callback, NULL);
    timer_start(TIMER_DEV);

    // Keep the main loop running
    while (1) {
        // Idle loop
    }
    return 0;
}