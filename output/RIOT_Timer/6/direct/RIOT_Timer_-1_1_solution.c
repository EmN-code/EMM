#include <ztimer.h>
#include "timex.h"
#include "thread.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Substitute PORT_A with the correct port for your board
#define INTERVAL (1U * US_PER_SEC)  // 1 second interval

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    ztimer_start(ZTIMER_MSEC, &timer, INTERVAL);

    while (1) {
        thread_yield();  // Let the RIOT scheduler do its work
    }
    return 0;
}