#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "mbox.h"

static mbox_t my_mbox;
static char my_mbox_queue[32];

event_mbox_event_t event;

evtimer_t my_timer;

void my_event_callback(void *arg) {
    // Example callback function for handling the periodic event
    printf("Periodic event triggered\n"); // pseudo function for printing
}

int main(void)
{
    /* Initialize the mbox */
    mbox_init(&my_mbox, my_mbox_queue, sizeof(my_mbox_queue));

    /* Initialize event timer */
    evtimer_init_mbox(&my_timer);
    
    /* Set up periodic event */
    evtimer_mbox_event_init(&event, my_event_callback, NULL, 1000000);
    evtimer_add_mbox(&my_timer, &event, &my_mbox);
    
    /* Main loop */
    while (1) {
        evtimer_process(&my_timer);
    }
    
    return 0;
}

// Missing evtimer_mbox_event_init function imitating its functionality:
static inline void evtimer_mbox_event_init(evtimer_mbox_event_t *event,
                                           void (*callback)(void *),
                                           void *arg,
                                           uint32_t offset) {
    
    event->event.callback = callback;
    event->event.arg = arg;
    event->event.offset_ticks = offset;
}