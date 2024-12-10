#include <ztimer/periodic.h>
#include <ztimer.h>
#include <stdio.h>
#include <inttypes.h>

// callback function to be executed periodically
static void my_callback(void *arg) {
    (void)arg;
    printf("Callback function called.\n");
}

int main(void) {
    // define a periodic timer
    ztimer_periodic_t my_periodic_timer;

    // initialize the periodic timer
    ztimer_periodic_init(&my_periodic_timer, ZTIMER_MSEC, my_callback, NULL, 1000);

    // start the periodic timer
    ztimer_periodic_start(&my_periodic_timer);

    // This will keep the program running and dispatching periodically
    while (1) {
        ztimer_periodic_dispatch(&my_periodic_timer);
    }
    return 0;
}