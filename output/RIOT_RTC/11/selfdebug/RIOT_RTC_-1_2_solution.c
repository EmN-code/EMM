#include "periph_conf.h"
#include "periph_cpu.h"
#include "stdio.h"
#include "event/callback.h"
#include "periph/rtc.h"

static void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_time_t time = {
        .tm_year = 2024,
        .tm_mon = 5,  // June (0-based, so January is 0)
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time\n");
        return 1;
    }

    rtc_time_t alarm_time;
    rtc_get_time(&alarm_time);

    // Set the alarm 5 seconds after the current time
    alarm_time.tm_sec += 5;

    if (rtc_set_alarm(&alarm_time, rtc_alarm_cb, NULL) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    printf("Alarm will trigger in 5 seconds\n");

    while (1) {
        // Infinite loop to keep the program running
    }

    return 0;
}