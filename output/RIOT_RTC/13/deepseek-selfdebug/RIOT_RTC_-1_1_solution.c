
#include "xtimer.h"
#include "periph/rtc.h"
#include "timex.h"
#include "fmt.h"

#define ALARM_DELAY (5 * US_PER_SEC)  // 5 seconds

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time;

    // Initialize RTC
    rtc_init();

    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900;  // years since 1900
    time.tm_mon = 5;             // months since January (0-11)
    time.tm_mday = 1;            // day of the month (1-31)
    time.tm_hour = 0;            // hours since midnight (0-23)
    time.tm_min = 0;             // minutes after the hour (0-59)
    time.tm_sec = 0;             // seconds after the minute (0-60)

    rtc_set_time(&time);

    // Set an alarm to trigger after 5 seconds
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        xtimer_sleep(1);  // Sleep for 1 second to avoid busy-waiting
    }

    return 0;
}
