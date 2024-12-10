#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define INTERVAL (1 * US_PER_SEC)

static void timer_callback(void *arg) {
    (void)arg;
    static bool led_state = false;
    
    if (led_state) {
        gpio_clear(GPIO_PIN(PORT_A, 1));
    } else {
        gpio_set(GPIO_PIN(PORT_A, 1));
    }
    led_state = !led_state;
    printf("LED state: %d\n", led_state);
}

int main(void) {
    if (gpio_init(GPIO_PIN(PORT_A, 1), GPIO_OUT)) {
        printf("Error initializing GPIO\n");
        return 1;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        timer_callback(NULL);
    }

    return 0;
}
