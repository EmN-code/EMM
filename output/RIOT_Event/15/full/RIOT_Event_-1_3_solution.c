#include "xtimer.h"
#include "ztimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "MQTTClient.h"

#define INTERVAL (1U * 1000000U)  // interval in microseconds

void callback(void) {
    printf("Callback function called!\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    while (1) {
        callback();
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }
    return 0;
}