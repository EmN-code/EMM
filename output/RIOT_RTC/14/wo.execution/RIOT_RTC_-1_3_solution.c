#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define ALARM_INTERVAL_SEC 5

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time = { .tm_year = 2024 - 1900,  // Year since 1900
                       .tm_mon = 6 - 1,        // 0-based month
                       .tm_mday = 1,           // Day of the month
                       .tm_hour = 0,
                       .tm_min = 0,
                       .tm_sec = 0 }; 

    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return -1;
    }

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return -1;
    }
    
    // Set the alarm to trigger after the ALARM_INTERVAL_SEC
    xtimer_sleep(ALARM_INTERVAL_SEC);
    alarm_callback(NULL);

    return 0;
}