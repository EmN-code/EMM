#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "periph/gpio.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)  // 1 second interval
#define LED_PIN GPIO_PIN(0, 10)           // Assuming the LED is connected to pin 10, port 0

static void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
    puts("LED toggled!");
}

int main(void)
{
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Could not initialize LED pin");
        return 1;
    }
    
    ztimer_clock_t *clock = ZTIMER_MSEC;
    ztimer_t timer;

    ztimer_set_msg_periodic(clock, &timer, TIMER_INTERVAL, scheduler_pid, NULL, NULL, 0);

    puts("Timer set, entering main loop...");

    while (1) {
        ztimer_sleep(clock, TIMER_INTERVAL);  // Sleep to simulate doing other work
    }

    return 0;
}