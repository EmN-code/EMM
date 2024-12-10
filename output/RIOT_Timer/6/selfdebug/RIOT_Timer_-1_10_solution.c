#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN     GPIO_PIN(PORT, PIN)  // Replace 'PORT' and 'PIN' with actual port and pin number

static void led_toggle(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled!\n");
}

int main(void) {
    printf("RIOT ESP32 timer example\n");

    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error to initialize pin\n");
        return 1;
    }

    // Create a timer that triggers every second
    xtimer_ticks32_t interval = xtimer_nsec_to_ticks32(1000000000);  // 1 second
    while (1) {
        led_toggle(NULL);
        xtimer_periodic_wakeup(&interval, interval);
    }

    return 0;
}