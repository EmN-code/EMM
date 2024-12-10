#include "ztimer/ztimer.h"
#include "stdio_base.h"
#include "event/callback.h"
#include <kernel_defines.h>

static ztimer_t rtc_timer;

void set_rtc_alarm(void)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    puts("Setting RTC alarm example\n");

    rtc_init();

    ztimer_set(ZTIMER_MSEC, &rtc_timer, 5000); // set timer for 5 seconds
    rtc_timer.callback = set_rtc_alarm;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}