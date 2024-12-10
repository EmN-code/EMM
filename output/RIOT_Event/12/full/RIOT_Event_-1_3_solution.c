// Include necessary RIOT headers in descending order
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "time_units.h"
#include "msg.h"
#include "mbox.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "event/callback.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

// Define thread stack size
#define THREAD_STACKSIZE_DEFAULT 1024

// Declare an event timer
static evtimer_msg_t evtimer;

// The callback function that will be executed periodically
static void periodic_event(void) {
    printf("Event triggered every second.\n");
}

// Thread function to handle events
void *event_thread(void *arg) {
    (void)arg;
    evtimer_msg_event_t event;
    msg_t msg_queue[8];

    // Initialize the event message system
    msg_init_queue(msg_queue, 8);

    // Initialize event timer with msg handling
    evtimer_init_msg(&evtimer);
    
    // Setup the callback event
    event.event.callback = periodic_event;
    event.event.param = NULL;
    event.msg.content.value = 0;

    // Add the event to the timer with interval 1 second
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    // Event loop to receive and process messages
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        evtimer_handler_msg(&evtimer, &msg);
    }

    return NULL;
}

// Main Application
int main(void) {
    printf("Starting periodic event example on ESP32 using RIOT.\n");

    // Creating a thread to handle periodic event
    char stack[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t event_thread_pid = thread_create(stack, sizeof(stack),
                                                  THREAD_PRIORITY_MAIN - 1,
                                                  THREAD_CREATE_STACKTEST,
                                                  event_thread,
                                                  NULL, "event_thread");
    assert(event_thread_pid > KERNEL_PID_UNDEF);

    // Start main loop
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}