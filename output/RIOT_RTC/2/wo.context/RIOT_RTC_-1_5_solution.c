#include <stdio.h>
#include <time.h>
#include <xtimer.h>
#include <timex.h>
#include <periph/rtc.h>
#include <msg.h>

void alarm_callback(void *arg) {
    (void)arg;  // Ignore unused argument warning
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon  = 5,  // 0 based, so June
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };

    if (rtc_init() != RTC_OK) {
        puts("RTC initialization failed");
        return 1;
    }

    if (rtc_set_time(&time) != RTC_OK) {
        puts("Failed to set time");
        return 1;
    }

    // Schedule an alarm after 5 seconds
    struct tm alarm_time;
    rtc_get_time(&alarm_time);
    timex_t alarm_offset = { .seconds = 5, .microseconds = 0 };
    TIMEX_ADD(alarm_time, alarm_offset, &alarm_time);

    if (rtc_set_alarm(&alarm_time, alarm_callback, NULL) != RTC_OK) {
        puts("Failed to set alarm");
        return 1;
    }

    puts("RTC time set to: June 1, 2024, and alarm in 5 seconds");

    // Prevent the program from exiting
    msg_t msg;
    msg_receive(&msg);

    return 0;
}