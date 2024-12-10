#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "event.h"
#include "core.h"
#include "cheri.h"
#include "periph/rtc.h"

#define ALARM_OFFSET 5

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    // Set RTC date to 2024-06-01
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1; // Month, 0 - january
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0; // Midnight
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&time);

    // Set alarm 
    struct tm alarm_time;
    memcpy(&alarm_time, &time, sizeof(struct tm));
    alarm_time.tm_sec += ALARM_OFFSET;

    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}