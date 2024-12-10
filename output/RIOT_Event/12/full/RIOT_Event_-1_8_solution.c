#include <ztimer.h>
#include <xtimer.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <evtimer.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>

// Function to be called every second
void on_event_periodic(void *arg)
{
    (void)arg; // avoid unused parameter warning
    printf("Periodic event triggered every second!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    kernel_pid_t pid = thread_getpid();

    // Initialize the event timer for message handling
    evtimer_init_msg(&evtimer);

    // Populate the event structure
    event.event.interval = 1000000U; // 1 second in microseconds
    event.event.callback = on_event_periodic;

    // Add event to the evtimer
    evtimer_add_msg(&evtimer, &event, pid);

    // Start the timer loop
    while (1) {
        // This would typically run in its own thread
        evtimer_run(&evtimer);
    }

    return 0;
}