#include <ztimer.h>
#include <stdio.h>
#include <msg.h>

static void _event_periodic(void *arg) {
    (void)arg;
    printf("Periodic event triggered.\n");
}

int main(void) {
    printf("Starting periodic event example.\n");
    
    ztimer_periodic_t periodic_timer;
    
    // Initialize the periodic timer with correct function pointer
    ztimer_periodic_init(ZTIMER_USEC, &periodic_timer, _event_periodic, NULL, ZTIMER_USEC_BASE);

    // Start the timer with 1-second period
    ztimer_periodic_start(&periodic_timer, 1 * ZTIMER_USEC_BASE);

    // Main loop
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}