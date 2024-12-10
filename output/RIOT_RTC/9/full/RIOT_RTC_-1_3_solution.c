#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

void alarm_triggered(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC device
    rtc_init();
    
    // Set the date and time to June 1, 2024, 00:00:00
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 6 - 1,        // Month, starting from 0
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Validate time setting
    while (rtc_set_time(&time) != 0) {
        printf("Failed to set time, retrying...\n");
        xtimer_sleep(1);
    }

    // Set an alarm for 5 seconds after the set time
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    // Validate alarm setting
    while (rtc_set_alarm(&alarm_time, alarm_triggered) != 0) {
        printf("Failed to set alarm, retrying...\n");
        xtimer_sleep(1);
    }

    // Keep the program running to wait for the alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}