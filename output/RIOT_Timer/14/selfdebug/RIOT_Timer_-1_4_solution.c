#include "xtimer.h"
#include "stdio.h"
#include "periph/gpio.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)
#define LED_PIN GPIO_PIN(PORT_A, 0)  // Assuming LED is connected on PORT_A, PIN_0

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled!\n");
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        timer_callback(NULL);
    }
    return 0;
}