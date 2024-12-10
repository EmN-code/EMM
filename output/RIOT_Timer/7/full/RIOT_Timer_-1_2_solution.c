#include "ztimer.h"
#include "xtimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)
#define LED_PIN 2

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    static int led_status = 0;

    // Toggle the LED status
    if (led_status) {
        // Code to turn off LED (for illustration, replace with actual function)
        printf("LED OFF\n");
    } else {
        // Code to turn on LED (for illustration, replace with actual function)
        printf("LED ON\n");
 }

    led_status = !led_status;
}

int main(void) {
    printf("Initializing timer...\n");

    ztimer_set_msg(ZTIMER_SEC, &timer, TIMER_INTERVAL, timer_callback, NULL);

    while (1) {
        ztimer_sleep(ZTIMER_SEC, TIMER_INTERVAL);
    }

    return 0;
}