#include <xtimer.h>
#include <periph/gpio.h>

#define INTERVAL_US (1000000U)

void timer_callback(void *arg) {
    (void)arg;
    static bool led_status = false;
    
    if (led_status) {
        gpio_clear(GPIO_PIN(PORT_A, 0));
    } else {
        gpio_set(GPIO_PIN(PORT_A, 0));
    }
    led_status = !led_status;
}

int main(void) {
    gpio_init(GPIO_PIN(PORT_A, 0), GPIO_OUT);

    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_US);
        timer_callback(NULL);
    }

    return 0;
}