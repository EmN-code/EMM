#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "rtc.h"
#include "periph/rtc.h"
#include "kernel.h"
#include "isrpipe.h"

void alarm_handler(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm time;  
    rtc_alarm_t alarm;
    int rc;

    /* Set date to June 1, 2024 */
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 5;             // Month 0-11 (June)
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    while (1) {
        rc = rtc_set_time(&time);
        if (rc < 0) {
            printf("RTC set time failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        break;
    }

    /* Setup alarm after 5 seconds */
    alarm = rtc_alarm_next();
    alarm.time = rtc_get_alarm_time() + 5;
    alarm.cb = alarm_handler;
    alarm.arg = NULL;

    while (1) {
        rc = rtc_set_alarm(&alarm);
        if (rc < 0) {
            printf("RTC set alarm failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        break;
    }

    printf("RTC initialized and alarm set. System time set to June 1, 2024. Awaiting alarm...\n");

    while (1) {
        /* Main logic loop, waiting for alarm */
        xtimer_sleep(1);
    }
    return 0;
}