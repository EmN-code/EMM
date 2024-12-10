#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"

#define INTERVAL (1U * US_PER_SEC)

static void _callback(void *arg) {
    (void)arg;
    // Simulating a periodic task
    puts("Timer callback executed.");
}

int main(void) {
    ztimer_t timer;
    timer.callback = _callback;
    timer.arg = NULL;

    while (1) {
        xtimer_usleep(INTERVAL);
        _callback(NULL);
    }

    return 0;
}