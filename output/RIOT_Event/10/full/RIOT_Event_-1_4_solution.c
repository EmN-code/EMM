#include <xtimer.h>
#include <ztimer.h>
#include <time_units.h>
#include <timex.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

/* Define a global timer and event structure */
static evtimer_t my_ev_timer;
static evtimer_mbox_event_t my_event;
static mbox_t my_mbox;
static msg_t mbox_queue[8];  // Queue needs to be a power of two

static void on_timeout(void *arg)
{
    (void)arg;  /* Avoid unused variable warning */
    /* Action to be performed every second */
    printf("Event triggered\n");
}

void init_event_timer(void)
{
    /* Initialize the mailbox */
    mbox_init(&my_mbox, mbox_queue, 8);
    
    /* Initialize the event timer */
    evtimer_init_mbox(&my_ev_timer);
    
    /* Configure the event to trigger on timeout */
    evtimer_mbox_event_t temp_event;
    temp_event.event.callback = on_timeout;

    /* Add the event to the timer with the mbox */
    evtimer_add_mbox(&my_ev_timer, &temp_event, &my_mbox);
}

int main(void)
{
    printf("Initializing event timer example...\n");
    
    init_event_timer();

    /* Start the event loop */
    while (true) {
        msg_t msg;
        msg_receive(&msg);
        ztimer_sleep(ZTIMER_MSEC, 1000);  // Sleep for 1000 milliseconds (1 second)
    }

    return 0;
}