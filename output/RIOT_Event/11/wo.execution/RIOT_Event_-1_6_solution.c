#include "xtimer.h"
#include "event/callback.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define INTERVAL_SEC   (1U)

static void callback(void *arg) {
    (void)arg;
    // Callback function code that runs every second
}

int main(void) {
    evtimer_msg_t evtimer;
    evtimer_msg_event_t event;
    kernel_pid_t pid = thread_getpid();

    // Initialize the event timer for IPC message events
    evtimer_init_msg(&evtimer);

    // Setup periodic event
    evtimer_add_msg(&evtimer, &event, pid);

    xtimer_sleep(INTERVAL_SEC);  // Sleep for 1 second delay

    while (1) {
        // Wait for the next event for this current thread
        msg_t msg;
        msg_receive(&msg);
        callback(NULL);
    }

    return 0;
}