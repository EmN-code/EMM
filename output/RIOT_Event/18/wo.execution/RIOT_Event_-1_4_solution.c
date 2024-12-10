#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "msg.h"
#include "mbox.h"
#include "unistd.h"
#include "time_units.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "event/callback.h"
#include "event/timeout.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;

void my_event_callback(void* arg)
{
    (void)arg;
    printf("Callback triggered\n");
}

int main(void)
{
    printf("Starting event timer\n");
    
    evtimer_init_msg(&evtimer);

    evtimer_add(&evtimer, INTERVAL);

    while (1) {
        evtimer_wait(&evtimer);
        my_event_callback(NULL);
    }

    return 0;
}