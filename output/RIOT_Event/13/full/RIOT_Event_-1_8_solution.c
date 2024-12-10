#include <kernel_defines.h>
#include <event/callback.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <xtimer.h>
#include <null.h>
#include <evtimer_mbox.h>

static evtimer_t evtimer;
static evtimer_event_t evtimer_event;

void callback(void *arg) {
    (void)arg;
    printf("Callback function executed.\n");
}

int main(void) {
    // Initialize the event timer
    evtimer_init(&evtimer, callback);

    // Set the periodic event with a 1-second interval (1,000,000 microseconds)
    evtimer_set_periodic(&evtimer_event, &evtimer, 1000000);

    // Loop to simulate event checking and invoking callback
    while (1) {
        evtimer_run(&evtimer);
        xtimer_sleep(1);
    }
    return 0;
}