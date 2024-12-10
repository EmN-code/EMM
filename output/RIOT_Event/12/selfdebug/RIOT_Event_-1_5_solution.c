#include <ztimer/periodic.h>
#include <ztimer.h>
#include <event.h>

void event_periodic(void *arg) {
    (void)arg;
    printf("Periodic event callback triggered!\n");
}

int main(void) {
    ztimer_periodic_t periodic;
    
    // Initialization of the periodic timer
    ztimer_clock_t *clock = ZTIMER_USEC;
    ztimer_periodic_init(clock, &periodic, event_periodic, NULL, 1000000);

    // Start the periodic timer
    ztimer_periodic_start(&periodic);
    
    printf("Started periodic event callback every second.\n");
    
    // Main event loop
    while(1) {
        ztimer_periodic_wakeup(&periodic);
    }

    return 0;
}