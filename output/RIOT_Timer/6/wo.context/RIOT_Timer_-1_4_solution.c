#include <ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT, 2) // Assumed LED_PIN port and pin

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled!\n");
    ztimer_set(ZTIMER_SEC, &timer, 1U, timer_callback, NULL);
}

int main(void) {
    puts("RIOT Timer Example");
    gpio_init(LED_PIN, GPIO_OUT);
    ztimer_set(ZTIMER_SEC, &timer, 1U, timer_callback, NULL);

    while(1) {
        thread_yield();
    }
    return 0;
}