#include <timex.h>
#include <thread.h>
#include <periph/gpio.h>
#include <xtimer.h>

#define LED_PIN     GPIO_PIN(PORT_A, 0)

void timer_callback(void *arg) {
    (void)arg;
    gpio_set(LED_PIN);
    xtimer_sleep(1);
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    xtimer_ticks32_t last_wakeup = xtimer_now();
    
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, 1000000);
        timer_callback(NULL);
    }
    return 0;
}