
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval
#define LED_PIN 5                        // Assuming LED is connected to GPIO 5

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED logic here (simulated)
    // For simulation, we will just print a message
    printf("LED toggled\n");
}

int main(void) {
    // Initialize message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize event timer
    evtimer_init_mbox(&evtimer, &mbox);

    // Set up the timer to trigger every second
    evtimer_add_msg(&evtimer, (xtimer_ticks32(TIMER_INTERVAL)), (void *)0, timer_callback, NULL);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle the message (if needed)
    }

    return 0;
}
