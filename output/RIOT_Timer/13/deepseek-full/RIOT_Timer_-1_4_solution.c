
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <stdlib.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <unistd.h>
#include <stdatomic.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval
#define LED_PIN 5  // Assuming LED is connected to GPIO 5

static volatile bool led_state = false;

static void timer_callback(void *arg)
{
    (void)arg;
    led_state = !led_state;
    printf("LED state: %s\n", led_state ? "ON" : "OFF");
}

int main(void)
{
    ztimer_t timer;
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
        timer_callback(NULL);
    }

    return 0;
}
