
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_msg_t evtimer;
static msg_t msg_queue[8];

void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED logic here
    // For example, if using a specific LED pin:
    // gpio_toggle(LED0_PIN);
    // Note: gpio_toggle is blacklisted, so this is a placeholder
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the timer to trigger every second
    evtimer_add_msg(&evtimer, timex_set(0, TIMER_INTERVAL), timer_callback, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle the message if needed
    }

    return 0;
}
