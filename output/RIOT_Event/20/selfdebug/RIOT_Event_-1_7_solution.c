#include <ztimer.h>
#include <stdio.h>
#include <stdlib.h>

void event_periodic(void *arg) {
    (void)arg;  // To prevent unused variable warning
    printf("Periodic event triggered\n");
}

int main(void) {
    // Initialize timer with the appropriate timer object.
    ztimer_t timer;
    ztimer_init(&timer, ZTIMER_MSEC);  

    // Set the timer to invoke the event_periodic every second (1000 milliseconds).
    ztimer_set_msg(&timer, ZTIMER_MSEC, 1000, event_periodic);
    
    while (1) {
        // Sleep for one second to simulate periodic behavior.
        ztimer_sleep(ZTIMER_MSEC, 1000);
        printf("Main loop iteration\n");
    }

    return 0;
}