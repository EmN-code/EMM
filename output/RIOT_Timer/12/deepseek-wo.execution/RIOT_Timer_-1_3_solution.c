
#include <evtimer_mbox.h>
#include <msg.h>
#include <ztimer.h>
#include <mma8x5x_regs.h>
#include <timex.h>
#include <mbox.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_msg.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void _led_toggle(void) {
    // Placeholder for LED toggle logic
    // This function should contain the logic to toggle the LED
    // For example, if using GPIO, it would be something like:
    // gpio_toggle(LED0_PIN);
}

static void _evtimer_mbox_handler(event_t *event) {
    (void)event;
    _led_toggle();
}

int main(void) {
    evtimer_t evtimer;
    msg_t msg_array[8];

    msg_init_queue(msg_array, 8);
    evtimer_init_mbox(&evtimer);

    while (1) {
        xtimer_usleep(TIMER_INTERVAL);
        evtimer_add_msg(&evtimer, NULL, NULL, 0);
    }

    return 0;
}
