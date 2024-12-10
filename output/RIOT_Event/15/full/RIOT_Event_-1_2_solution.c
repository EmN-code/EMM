#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "thread.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#include <stdio.h>

void event_periodic(void)
{
    printf("Periodic event triggered\n");
}

int main(void)
{
    ztimer_t timer;
    uint32_t period = ZTIMER_SEC;  // Set event period to 1 second

    ztimer_set_msg(&timer, period, thread_getpid());
    
    while (1) {
        event_periodic();
        ztimer_sleep(ZTIMER_SEC);
    }

    return 0;
}