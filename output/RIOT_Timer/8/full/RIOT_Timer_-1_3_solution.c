#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "unistd.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mbox.h"

#define LED_PIN GPIO_PIN(PORT_B, 0)
#define TIMER_INTERVAL 1 // in seconds

void timer_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    gpio_toggle(LED_PIN); // Toggle LED state
    printf("LED toggled\n");
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    while (1) {
        ztimer_set(ZTIMER_SEC, &timer, TIMER_INTERVAL);
        pause(); // Puts the calling thread to sleep
    }

    return 0;
}