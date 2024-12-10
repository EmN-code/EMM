#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#define ALARM_OFFSET_SEC 5

void alarm_callback(void)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    ztimer_now_t alarm_time;
    ztimer_clock_t clock = ZTIMER_MSEC;

    printf("Setting RTC to June 1, 2024 and setting an alarm for 5 seconds later.\n");
    
    // Simulated RTC setup
    struct tm rtc_time = {0};
    rtc_time.tm_year = 2024 - 1900;
    rtc_time.tm_mon = 5; // June
    rtc_time.tm_mday = 1;

    // Set the initial time (simulated)
    // Assuming setting of current time in rtc is done
    
    // Calculate the alarm time
    alarm_time = ztimer_now(clock) + ALARM_OFFSET_SEC * MS_PER_SEC;

    // Set the alarm callback
    ztimer_set(clock, alarm_time, (ztimer_cb_t)alarm_callback, NULL);

    printf("Alarm set for 5 seconds from now.\n");

    while (1) {
        // Main loop
    }

    return 0;
}