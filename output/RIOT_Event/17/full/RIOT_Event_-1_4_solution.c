#include "event/thread.h"
#include "event/callback.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "mma8x5x_regs.h"
#include "mbox.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdlib.h"
#include "thread.h"
#include "time_units.h"
#include "timex.h"
#include "unistd.h"
#include "xtimer.h"
#include "ztimer.h"

void event_periodic(void *arg)
{
    printf("Event periodic callback triggered\n");
    (void) arg; // To avoid unused variable warning
}

int main(void)
{
    // Assume evtimer_msg.h or evtimer_mbox.h provides event_periodic struct or function
    
    // Initialize the event struct/event and set the callback
    evtimer_periodic_init(&event_periodic, NULL);
    
    // Assuming we have the struct or function defined
    ztimer_set_periodic(ZTIMER_SEC, 1*US_PER_SEC, (ztimer_cb_t) event_periodic, NULL);

    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}