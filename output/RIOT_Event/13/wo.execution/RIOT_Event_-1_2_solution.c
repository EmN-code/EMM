#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <stdio.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Callback function called every second.\n");
}

int main(void)
{
    ztimer_t timer;
    ztimer_set_msg(&timer, ZTIMER_MSEC, 1000, periodic_callback, NULL);
    
    puts("Timer initialized to trigger every second.");
    
    while (1) {
        ztimer_sleep(ZTIMER_USEC, 1000000); // Sleep for 1 second
    }

    return 0;
}