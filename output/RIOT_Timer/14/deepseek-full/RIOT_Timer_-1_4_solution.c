
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval
#define LED_PIN 5  // Assuming LED is connected to GPIO 5

static msg_t msg_queue[8];
static evtimer_msg_t evtimer;
static evtimer_msg_event_t event;

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 1;  // Signal to toggle LED
    msg_send(&msg, thread_getpid());
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the timer event
    event.event.callback = timer_callback;
    event.event.arg = NULL;
    event.msg.sender_pid = thread_getpid();

    // Add the event to the event timer
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    while (1) {
        msg_t msg;
        msg_receive(&msg);

        if (msg.content.value == 1) {
            // Toggle LED logic here (assuming GPIO toggle function is available)
            // For example: gpio_toggle(LED_PIN);
        }
    }

    return 0;
}
