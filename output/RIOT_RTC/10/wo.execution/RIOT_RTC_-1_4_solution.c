#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "unistd.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "thread.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "event/thread.h"
#include "kernel_defines.h"
#include "time_units.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize ztimer
    ztimer_clock_t zclock = ZTIMER_USEC;
    
    // Set after 5 seconds from current time
    uint32_t delay = 5 * 1000000U; // delay in microseconds

    // Call ztimer callback after 5 seconds
    ztimer_set(zclock, xtimer_now() + delay, alarm_callback, NULL);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}