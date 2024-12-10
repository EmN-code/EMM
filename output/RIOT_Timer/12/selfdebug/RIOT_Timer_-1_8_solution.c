#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2)  // Assuming the LED is connected to GPIO port 0, pin 2

void timer_callback(void *arg) {
    (void)arg;
    static int led_state = 0;
    led_state = !led_state;  // Toggle LED state
    gpio_write(LED_PIN, led_state);
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    
    xtimer_ticks32_t last_wakeup = xtimer_now();
    
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, 1 * US_PER_SEC);
        timer_callback(NULL);
    }

    return 0;
}