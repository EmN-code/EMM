#include <ztimer.h>
#include <timex.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 5)

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the timer to trigger every second
    timer.callback = timer_callback;
    timer.arg = NULL;
    ztimer_set(&timer, ZTIMER_SEC, 1);

    // Running the main loop
    while (1) {
        // IDLE loop
    }
    return 0;
}