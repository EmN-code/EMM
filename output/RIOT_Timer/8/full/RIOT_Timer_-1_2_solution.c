#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include <stdio.h>

#define TIMER_INTERVAL 1 

void timer_callback(void *arg)
{
    (void)arg;
    // To simulate the LED being toggled.
    static int led_state = 0;
    led_state = !led_state;
    printf("LED is %s\n", led_state ? "ON" : "OFF");
}

int main(void)
{
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    timer.offset = 0;

    while (1) {
        ztimer_set(ZTIMER_SEC, &timer, TIMER_INTERVAL);
        ztimer_spin(ZTIMER_SEC, &timer);
    }
    return 0;
}