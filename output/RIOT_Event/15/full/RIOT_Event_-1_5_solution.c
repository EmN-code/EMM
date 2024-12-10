#include <mbo.evtimer_mbox.h>
#include <mbox.h>
#include <kernel_defines.h>

// Global Variables
static evtimer_t event_timer;
static evtimer_msg_event_t event;
static msg_t msg_queue[8];  // Message queue

void my_callback(adapter_msg pid) {
    puts("Event triggered!");
}

int main(void) {
    puts("Starting event timer...");

    // Initialize the timer
evqueue_event_t evt;
    evtimer_init_msg(&event_timer);

    // Prepare event type for the timer
    event.super.interval = 1000000U;  // 1 second in microseconds
    event.super.callback_sent = my_callback;

    // Add event to timer
    evtimer_add(&event_timer, &event);

    // Run event-loop
    while (1) {
        event_trigger(&event_timer, msg_queue);
    }

    return 0;
}
