
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 18)

void toggle_led(void) {
    gpio_toggle(LED_PIN);
}

void init_led(void) {
    gpio_init(LED_PIN, GPIO_OUT);
}

int main(void) {
    init_led();

    while (1) {
        toggle_led();
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return 0;
}
