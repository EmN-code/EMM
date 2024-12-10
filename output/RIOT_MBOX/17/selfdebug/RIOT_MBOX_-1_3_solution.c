#include <xtimer.h>
#include <mbox.h>
#include <evtimer.h>

#define INTERVAL    (1U * US_PER_SEC)

static msg_t msg_queue[8];
evttimer_t timer_evt;
evttimer_msg_event_t timer_event;
static mbox_t mbox;

static void _evtimer_mbox_handler(evtimer_event_t *event) {
    puts("Timer event occurred");
}

int main(void) {
    // Initialize message box
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_t));
    
    // Initialize event timer
    evtimer_init_mbox(&timer_evt);
    
    // Configure event
    timer_event.event.period = INTERVAL;
    timer_event.msg.type = 0; // Set to 0 or any specific type
    timer_event.msg.content.ptr = NULL;
    
    // Add event to event timer
    evtimer_add_mbox(&timer_evt, (evtimer_mbox_event_t *)&timer_event, &mbox);
    
    // Start the event timer
    xtimer_sleep(10);

    return 0;
}