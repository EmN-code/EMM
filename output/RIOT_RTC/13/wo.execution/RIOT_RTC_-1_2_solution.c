#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "periph/rtc.h"

#include <stdio.h>

static void alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC hardware
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return -1;
    }

    // Set current date and time - assuming Jan 1, 2024, at 00:00:00
    struct tm time = { .tm_year = 2024 - 1900, // Years since 1900
                       .tm_mon = 5,            // June
                       .tm_mday = 1,           // 1st of June
                       .tm_hour = 0,
                       .tm_min = 0,
                       .tm_sec = 0 };

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time\n");
        return -1;
    }
    
    // Set an alarm to trigger in 5 seconds
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    if (rtc_set_alarm(&alarm_time, alarm_cb) != 0) {
        printf("Failed to set alarm\n");
        return -1;
    }

    // Keep the program running to wait for the alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}