#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "time.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

int main(void) {
    // Initialize RTC
    rtc_init();
    
    // Set the date and time: June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // tm_year is years since 1900
    time.tm_mon = 5;           // tm_mon is 0-based
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set an alarm to trigger after 5 seconds
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    // Convert the struct tm to timestamp
    time_t alarm_timestamp = mktime(&alarm_time);

    // Function to call when the alarm triggers
    void alarm_handler(void *arg) {
        (void)arg; // Unused variable
        printf("Alarm triggered!\n");
    }

    // Convert timestamp to rtc time
    rtc_alarm_t alarm;
    rtc_timestamp_to_alarm(alarm_timestamp, &alarm);

    // Set the alarm with a handler
    rtc_set_alarm(&alarm, alarm_handler, NULL);

    // Enter sleep
    while(1) {
        xtimer_sleep(10);
    }

    return 0;
}
