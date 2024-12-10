
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_msg.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static evtimer_msg_t evtimer;

void timer_callback(void *arg)
{
    (void)arg;
    // Toggle LED logic here
    // For simplicity, let's assume we have a function to toggle an LED
    // toggle_led();
    puts("LED toggled");
}

int main(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the timer to trigger every second
    evtimer_add_msg(&evtimer, timex_set(TIMER_INTERVAL, 0), timer_callback, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle the message if needed
    }

    return 0;
}
