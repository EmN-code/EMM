#include "ztimer.h"
#include "timex.h"
#include "periph/gpio.h"
#include "stdio.h"

#define LED_PIN GPIO_PIN(PORT_A, 2)

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error to initialize LED_PIN\n");
        return 1;
    }

    // Declare a timer variable
    ztimer_t timer;

    // Set the timer to trigger every second and toggle the LED
    ztimer_set_msg(ZTIMER_MSEC, &timer, 1000U, thread_getpid(), timer_cb, NULL);

    // Main loop that just sleeps
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}