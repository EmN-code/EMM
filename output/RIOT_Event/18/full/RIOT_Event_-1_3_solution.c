#include "xtimer.h"
#include "stdio.h"
#include "event/thread.h"
#include "event/callback.h"
#include "event/timeout.h"
#include "msg.h"
#include "stddef.h"
#include "string.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "time_units.h"

// Callback function that will be called periodically
event_t periodic_event;

// Simple function to illustrate periodic execution
void event_callback(event_t *event) {
    printf("Periodic event triggered!\n");
    
    // Schedule the next event
    evtimer_add_msg((evtimer_msg_t*)&periodic_event, 
                    (evtimer_msg_event_t*)event, 
                    thread_getpid());
}

int main(void) {
    // Timer interval: 1 second
    uint32_t interval = 1000000;

    // Initialize the event
    evtimer_init_msg((evtimer_t*)&periodic_event);
    periodic_event.callback = event_callback;

    // Start the periodic event
    evtimer_add_msg((evtimer_msg_t*)&periodic_event, 
                    (evtimer_msg_event_t*)&periodic_event, 
                    thread_getpid());

    return 0;
}