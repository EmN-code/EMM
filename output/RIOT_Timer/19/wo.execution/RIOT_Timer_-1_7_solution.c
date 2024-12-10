#include <ztimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <timex.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 2)

void toggle_led(void) {
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void) {
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error initializing GPIO\n");
        return 1;
    }

    ztimer_sleep(ZTIMER_MSEC, 1000);

    while (1) {
        toggle_led();
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}
