#include <ztimer.h>  // Importing ztimer.h for accessing ztimer related functionality
#include <msg.h>     // Importing msg.h for message passing between threads

#define PERIOD 1U  // Define the period for one second

static ztimer_t periodic_timer;  // Define a ztimer object

static void callback(void *arg) {
    (void)arg;  // Unused here
    puts("Timer callback invoked!");  // Output the message when callback is invoked
}

int main(void) {
    // Set up the timer structure/functions
    ztimer_set_handler(&periodic_timer, callback, NULL);
    
    // Start the timer in periodic mode, with specified period and current time
    ztimer_periodic_start(&periodic_timer, PERIOD * (1000 * 1000));
    
    while (1) {
      msg_t msg;
      msg_receive(&msg);  // Keep the main thread alive to process messages
    }
    
    return 0;
}