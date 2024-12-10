#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define ALARM_DELAY_SECONDS (5U)

static void alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Initialize the RTC
    while (rtc_init() != 0) {
        printf("RTC initialization failed, retrying...\n");
        xtimer_sleep(1);
    }

    printf("RTC initialized\n");

    // Set the desired time (June 1, 2024, 00:00:00)
    struct tm time;
    time.tm_year = 2024 - 1900;  // Years since 1900
    time.tm_mon = 5;             // Month, where 0 = January
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Define alarm time structure
    struct tm alarm_time = time;
    alarm_time.tm_sec += ALARM_DELAY_SECONDS;

    // Schedule alarm
    rtc_set_alarm(&alarm_time, &alarm_callback, NULL);

    // Enter a low-power sleep while waiting for the alarm
    while (1) {
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }

    return 0;
}
