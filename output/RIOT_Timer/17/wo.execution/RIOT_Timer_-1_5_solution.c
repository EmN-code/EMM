#include "kernel_defines.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include "msg.h"
#include "xtimer.h"
#include "paho_mqtt.h"
#include "zaper.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "timex.h"
#include "unistd.h"

// Define LED pin
#define LED_PIN GPIO_PIN(0, 2)

// Define timer for 1 second
#define TIMER_INTERVAL (1LU * US_PER_SEC)

// Timer callback function
void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize GPIO for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize a ztimer
    ztimer_t timer;
    evtimer_t evtimer;
    
    // Initialize event timer handling
    evtimer_init_msg(&evtimer);

    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start a periodic timer
    ztimer_periodic_wakeup(&timer, TIMER_INTERVAL);

    while (1) {
        // Run events associated with the timer
        evtimer_run(&evtimer);
        
        // Sleep the thread to wait for the next timer event
        thread_sleep();
    }

    return 0;
}
