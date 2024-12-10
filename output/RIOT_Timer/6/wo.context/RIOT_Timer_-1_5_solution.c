#include <ztimer.h>
#include <periph/gpio.h>
#include <stdio.h>

#define INTERVAL (1 * ZTIMER_MSEC_PER_SEC)
#define LED_PIN GPIO_PIN(PORT_A, 0) // Ensure PORT_A is correctly defined for your ESP32 setup

void timer_callback(void *arg) {
    (void)arg;
    printf("Timer callback triggered, turning LED on\n");
    gpio_set(LED_PIN);
}

int main(void) {
    ztimer_t timer;

    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error: unable to initialize LED pin\n");
        return 1;
    }

    timer.callback = timer_callback;
    timer.arg = NULL;
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }

    return 0;
}