#include "time_units.h"
#include "thread.h"
#include "unistd.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "MQTTClient.h"
#include "kernel_defines.h"
#include "event/thread.h"

void rtc_callback(void *arg) {
    (void)arg; /* Argument is not used */
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    rtc_set_time_t set_time;

    /* Set RTC time to June 1, 2024, 00:00:00 */
    timeinfo.tm_year = 2024 - 1900;
    timeinfo.tm_mon = 5;         /* Months since January 0-11 */
    timeinfo.tm_mday = 1;        /* Day of the month 1-31 */
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    set_time.time.tm = timeinfo;

    rtc_init();

    if (rtc_set_time(&set_time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    /* Alarm time: 5 seconds from now */
    struct tm alarm_time = timeinfo;
    alarm_time.tm_sec += 5;

    rtc_alarm_t alarm;
    alarm.time.tm = alarm_time;
    alarm.callback = rtc_callback;
    
    if (rtc_set_alarm(&alarm) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    /* Keep main thread alive to wait for the alarm */
    while (1) {
        thread_sleep();
    }

    return 0;
}