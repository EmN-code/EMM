#include <kernel_defines.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

void event_callback(void *arg) {
    (void)arg;
    // Event callback function logic here
    printf("Periodic callback triggered.\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    kernel_pid_t target_pid = KERNEL_PID_UNDEF;  // Use appropriate PID

    // Initialize the event timer for mbox
    evtimer_init_mbox(&evtimer);

    // Configure the event
    event.event.periodic.interval = ztimer_usec_from_sec(1); // 1 second
    event.event.periodic.callback = event_callback;
    event.event.periodic.arg = NULL;

    // Add the periodic event to the event timer
    evtimer_add_msg(&evtimer, &event, target_pid);

    // Start the event loop
    while (1) {
        evtimer_run(&evtimer);
    }

    return 0;
}