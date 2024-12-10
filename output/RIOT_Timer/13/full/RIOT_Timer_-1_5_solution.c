#include "xtimer.h"
#include "timex.h"
#include "msg.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "ztimer.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "stdatomic.h"
#include "stddef.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "time_units.h"
#include "unistd.h"

// Define Thread Stack and priority
#define THREAD_STACKSIZE_MAIN    (2 * THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

// Function to toggle LED state
void *toggle_led_thread(void *arg) {
    (void)arg;  // Unused parameter
    while (1) {
        printf("LED Toggled\n");
        // Logic to toggle LED here
        xtimer_sleep(1);  // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    // Event timer and event init
    evtimer_t evtimer;
    evtimer_mbox_event_t mbox_event;
    mbox_t mbox;
    msg_t queue[8];  // Define a message queue

    // Initialize mbox
    mbox_init(&mbox, queue, 8);

    // Init evtimer for mbox
    evtimer_init_mbox(&evtimer);

    // Set up the event
    evtimer_add_mbox(&evtimer, &mbox_event, &mbox);

    // Creating LED toggle thread
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, toggle_led_thread, NULL, "toggle_led_thread");

    while (1) {
        // Main Loop
        xtimer_sleep(1);
    }

    return 0;
}