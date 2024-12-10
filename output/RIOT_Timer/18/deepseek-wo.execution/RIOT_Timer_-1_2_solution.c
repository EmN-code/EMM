
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_msg_t evtimer;
static mbox_t mbox;
static msg_t msg_array[8];

void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED logic here
    // For simplicity, we assume LED is connected to GPIO 5
    // and we use a simple toggle mechanism
    static int led_state = 0;
    led_state = !led_state;
    // Toggle LED logic here
    // For simplicity, we assume LED is connected to GPIO 5
    // and we use a simple toggle mechanism
    // Note: This is a placeholder for actual GPIO toggle logic
    // which should be implemented according to the specific board
    // and GPIO library used.
}

int main(void) {
    // Initialize the message box
    mbox_init(&mbox, msg_array, sizeof(msg_array) / sizeof(msg_array[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer, &mbox);

    // Set up the timer to trigger every second
    evtimer_add_msg(&evtimer, TIMER_INTERVAL, timer_callback, NULL);

    // Main loop to process messages
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        ((msg_t_handler)msg.content.ptr)(msg.content.ptr);
    }

    return 0;
}
