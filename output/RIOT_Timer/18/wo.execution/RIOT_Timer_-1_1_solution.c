#include <ztimer.h>
#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>

#define BLINK_PERIOD (1U * US_PER_SEC) // 1 second period
#define LED_PIN GPIO_PIN(PORT_A, 5)   // Example LED pin

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    puts("Timer-based LED blink on ESP32");
    
    gpio_init(LED_PIN, GPIO_OUT);
    
    ztimer_t timer;
    ztimer_set(ZTIMER_USEC, &timer, BLINK_PERIOD, timer_callback, NULL);
    
    while (1) {
        ztimer_sleep(ZTIMER_USEC, BLINK_PERIOD);
    }
    return 0;
}