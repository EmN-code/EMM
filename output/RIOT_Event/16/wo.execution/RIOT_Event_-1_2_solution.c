#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <event/thread.h>
#include <event/msg.h>
#include <event/mbox.h>
#include <event/callback.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

void callback(void *arg) {
    // This is a simple callback function that gets called periodically
    printf("Callback executed\n");
}

int main(void) {
    // This main function initializes and starts a periodic timer
    ztimer_periodic_t periodic_timer;
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, 1000);  // Set to 1000 ms (1 second)

    while (1) {
        ztimer_periodic_start(&periodic_timer);
        callback(NULL);
        xtimer_sleep(1);  // Sleep for 1 second
    }
    return 0;
}