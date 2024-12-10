
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define LED_PIN             GPIO_PIN(0, 13)  // Assuming GPIO 13 on ESP32
#define TIMER_INTERVAL      (1 * US_PER_SEC) // 1 second interval

static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    xtimer_set(&timer, TIMER_INTERVAL);
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer to trigger the callback every second
    xtimer_set(&timer, TIMER_INTERVAL);

    return 0;
}
