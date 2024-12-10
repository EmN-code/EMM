#include <xtimer.h>
#include <ztimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <stdoi.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <mbox.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>

#define PERIOD 1000000 // 1 second expressed in microseconds

static evtimer_t evtimer;
static msg_t msg_queue[8];
static mbox_t mbox;

static void my_callback(void) {
    // Callback function to execute every second
    printf("Callback executed!\n");
}

static void *event_thread(void *arg){
    (void)arg;
    while(1){
        msg_t msg;
        // Wait indefinitely for a message
        msg_receive(&msg);
        my_callback();
    }
    return NULL;
}

int main(void) {
    // Initialize mailbox
    mbox_init(&mbox, msg_queue, 8);
    
    // Initialize the event timer with mbox
    evtimer_init_mbox(&evtimer);

    // Create a thread that will invoke callbacks
    thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN - 1, 0, event_thread, NULL, "event_thread");
    
    // Setting up an evtimer event
    evtimer_mbox_event_t event;
    evtimer_periodic_init(&event.event, PERIOD, NULL, NULL);
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Start execution
    evtimer_periodic_start(&event);

    return 0;
}