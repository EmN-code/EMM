
#include "xtimer.h"
#include "msg.h"
#include "ztimer.h"
#include "timex.h"
#include "mbox.h"
#include "time_units.h"
#include "evtimer_msg.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static mbox_t mbox;
static msg_t msg_queue[8];

void led_toggle(void) {
    // Simulate LED toggle logic here
    // This function should contain the logic to toggle the LED
    // For example, if you have a GPIO pin connected to an LED, you would toggle it here
    // Since GPIO functions are blacklisted, this is a placeholder for the actual logic
    printf("LED toggled\n");
}

void msg_handler(msg_t *msg) {
    (void)msg;  // Suppress unused parameter warning
    led_toggle();
}

int main(void) {
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_msg(&mbox);

    // Set up the timer to trigger every second
    evtimer_add_msg(&mbox, TIMER_INTERVAL, msg_handler);

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        if (_mbox_get(&mbox, &msg, 1)) {
            msg_handler(&msg);
        }
    }

    return 0;
}
