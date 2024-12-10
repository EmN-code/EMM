#include <ztimer.h>
#include <msg.h>
#include <timex.h>
#include <mbox.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <xtimer.h>
#include <time_units.h>

// Function to handle the LED task
void led_task(evttimer_event_t *event) {
    // Toggle the LED state
    printf("LED Toggled\n");
}

int main(void) {
    // LED Task Timer
    evtimer_t timer;

    // Initialize the timer
    evtimer_init_msg(&timer);
  
    // Setup the event
    evtimer_msg_event_t event;
    msg_t msg;
    event.msg = msg;

    // Set the callback
    timer.callback = led_task;

    // Add the LED event to the timer
    evtimer_add_msg(&timer, &event, KERNEL_PID_UNDEF);

    // Loop every second
    while (1) {
        usleep(1000000);  // Sleep for 1 second
        evtimer_add(&timer, &event.event); // Trigger the event
    }

    return 0;
}
