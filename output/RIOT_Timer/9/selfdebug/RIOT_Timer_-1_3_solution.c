#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_A, 2)
#define INTERVAL (1U * US_PER_SEC)

void toggle_led(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    while(1) {
        toggle_led();
        xtimer_sleep(INTERVAL / US_PER_MS);
    }
    return 0;
}