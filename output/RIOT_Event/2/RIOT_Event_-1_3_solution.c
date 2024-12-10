
#include <stdio.h>
#include "thread.h"
#include "periph/gpio.h"
#include "time_units.h"
#include "periph/rtc.h"

#define INTERVAL_SEC (1U)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    rtc_init();

    while (1) {
        periodic_callback(NULL);
        rtc_sleep(INTERVAL_SEC);
    }

    return 0;
}
