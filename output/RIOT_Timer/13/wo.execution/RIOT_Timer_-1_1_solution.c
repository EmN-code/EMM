#include "timer.h"
#include "thread.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_A, 5)

timer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    /* Initialize the GPIO pin connected to the LED */
    gpio_init(LED_PIN, GPIO_OUT);

    /* Set up the timer to trigger every second (1000000 microseconds) */
    timer_init(&timer, 1000000, timer_callback, NULL);
    timer_start(&timer);

    /* Run the event loop */
    while (1) {
        thread_yield();
    }

    return 0;
}