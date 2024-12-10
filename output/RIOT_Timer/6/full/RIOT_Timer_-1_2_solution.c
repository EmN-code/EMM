#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)
#define LED_PIN (12)  // Define a specific GPIO pin number 

void toggle_led(void *arg) {
    (void)arg;
    // Toggle the LED on the specified GPIO pin
    // Assume a function gpio_toggle exists
    // gpio_toggle(LED_PIN);
    // But as gpio_toggle and similar are blacklisted, this is a placeholder to help logic understanding
    printf("LED toggled\n");  // Placeholder for LED toggle indication
}

int main(void) {
    ztimer_t timer;
    ztimer_set_wakeup_periodic(ZTIMER_MSEC, &timer, TIMER_INTERVAL, toggle_led, NULL);
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }
    return 0;
}
