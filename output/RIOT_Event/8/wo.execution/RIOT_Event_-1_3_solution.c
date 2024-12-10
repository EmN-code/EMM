#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <time_units.h>
#include <stdlib.h>
#include <stdio.h>

// Global timer variable
event_t timer_event;

// Callback function for the event
event_callback_handler_t event_handler(event_t *event)
{
    puts("Event triggered!");
}

// Function to initialize and start the periodic event
void init_periodic_event(void)
{
    // Initialize the event
    evtimer_msg_event_t my_event;
    evtimer_t my_timer;
    evtimer_init_mbox(&my_timer);
    evtimer_add_mbox(&my_timer, &my_event, NULL);

    // Periodic execution every second
    xtimer_ticks32_t interval = xtimer_sec_to_ticks(1);

    // Initialize the event and link the callback
    event_periodic_init(&my_event.event, interval, event_handler);

    // Start the periodic event
    event_periodic_start(&my_event.event);
}

int main(void)
{
    init_periodic_event();
    return 0;
}