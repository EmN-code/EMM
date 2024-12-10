#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>

#define BLINK_PIN GPIO_PIN(0, 2) // Adjust for your setup

void timer_callback(void *arg) {
    (void)arg;
    static bool state = false;
    gpio_write(BLINK_PIN, state);
    state = !state;
    printf("LED state: %d\n", state);
}

int main(void) {
    gpio_init(BLINK_PIN, GPIO_OUT);

    timex_t interval = timex_set(1, 0);

    hwtimer_start(interval, timer_callback, NULL);

    while (1) {
        // Main loop can contain other logic if needed
    }

    return 0;
}