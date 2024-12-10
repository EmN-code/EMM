#include <xtimer.h>  // for timer functions
#include <stdio.h>    // for the printf function
#include <event/periodic.h>  // for periodic event handling

// Function to be called back every second
void callback(void *arg) {
    (void)arg;
    printf("Callback triggered\n");
}

// Main function
int main(void) {
    event_periodic_t periodic;
    event_periodic_init_cb(&periodic, callback, NULL, 1 * US_PER_SEC);
    event_periodic_start(&periodic);
    
    // Keep program running to demonstrate periodic events
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}