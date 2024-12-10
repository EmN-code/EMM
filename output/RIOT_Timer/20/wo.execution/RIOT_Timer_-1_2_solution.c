#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Change pin as per your hardware configuration

void led_blinker(void) {
    while (1) {
        gpio_toggle(LED_PIN);
        xtimer_sleep(1); // sleep for 1 second
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    
    thread_create(led_blinker_stack, sizeof(led_blinker_stack), THREAD_PRIORITY_MAIN - 1, 0, led_blinker, NULL, "led_blinker");

    
    return 0;
}