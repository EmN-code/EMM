#include <stdatomic.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <timex.h>
#include <event/thread.h>
#include <mbox.h>
#include <stddef.h>
#include <stdlib.h>
#include <ztimer.h>
#include <paho_mqtt.h>
#include <periph/gpio.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <stdio.h>
#include <event/periodic.h>
#include <kernel_defines.h>
#include <stdbool.h>
#include <periph/rtc.h>
#include <thread.h>
#include <time_units.h>
#include <msg.h>
#include <periph/i2c.h>

#define TIMER_INTERVAL (1U * 1000000U) // Set interval to 1 second in microseconds

ztimer_t periodic_timer; // Declare a global timer object

void periodic_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    printf("Periodic Callback Triggered!\n");
    ztimer_set_msg(ZTIMER_MSEC, &periodic_timer, TIMER_INTERVAL, thread_getpid());
}

int main(void) {
    printf("Initializing periodic timer...\n");
    // Initialize the timer
    ztimer_init();
    
    // Set timer to trigger every second
    ztimer_set_msg(ZTIMER_MSEC, &periodic_timer, TIMER_INTERVAL, thread_getpid());
    
    // Start the main event loop
    msg_t msg;
    while (1) {
        msg_receive(&msg); // Wait for messages (blocking call)
        periodic_callback(NULL); // Call the callback as messages are received
    }
    
    return 0;
}
