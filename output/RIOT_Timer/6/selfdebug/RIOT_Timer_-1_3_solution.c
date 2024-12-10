#include <timex.h>
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_B, 0)
#define TIMER_INTERVAL (1U * US_PER_SEC)

void *led_blink(void *arg) {
    (void)arg;
    while (1) {
        gpio_set(LED_PIN);
        xtimer_sleep(1);
        gpio_clear(LED_PIN);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    puts("LED Timer Example");

    gpio_init(LED_PIN, GPIO_OUT);

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  led_blink, NULL, "led_blink");

    return 0;
}