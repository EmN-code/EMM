#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <time_units.h>

#define ALARM_SECONDS 5

void alarm_callback(void *arg) {
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

    if (rtc_set_time(&time) != RTC_OK) {
        printf("Failed to set RTC time.\n");
        return 1;
    }

    rtc_alarm_t alarm;
    alarm.time = time;
    alarm.time.second += ALARM_SECONDS;
    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm) != RTC_OK) {
        printf("Failed to set RTC alarm.\n");
        return 1;
    }
    
    while (1) {
        // Main loop can have other functionalities
        usleep(100000);
    }
    return 0;
}