#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mbox.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#define ALARM_DELAY   (5U) // 5 seconds delay for the alarm

static void alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_time_t time;
    time.year = 2024;
    time.month = 6;
    time.day = 1;
    time.hour = 0;
    time.minute = 0;
    time.second = 0;

    // Initialize RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    // Set the current date and time
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set an alarm
    rtc_set_time_t alarm_time;
    rtc_get_time(&alarm_time); // fetch current time
    alarm_time.second += ALARM_DELAY; // set alarm delay

    if (rtc_set_alarm(&alarm_time, alarm_cb, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("Alarm set for 5 seconds from now\n");

    while(1) {
        // Placeholder for actual work, infinite loop
    }

    return 0;
}