#include <stdatomic.h>
#include <sys/evtimer_mbox.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <ztimer.h>
#include <sys/msg.h>
#include <sys/timex.h>
#include <stddef.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <sys/time_units.h>
#include <periph/i2c.h>
#include <stdlib.h>
#include <xtimer.h>
#include <sys/mbox.h>
#include <evtimer_msg.h>
#include <unistd.h>

int main(void) {
    struct tm time;
    rtc_alarm_cb_t alarm_cb;
    void *arg = NULL;

    // Set the initial RTC time to June 1, 2024
    time.tm_year = 2024 - 1900;
    time.tm_mon = 6 - 1;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set the RTC time
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set an alarm to trigger 5 seconds later
    alarm_cb = alarm_handler;
    
    time.tm_sec += 5; // Add 5 seconds to current time

    // Adjust the time structure in case of overflow
    mktime(&time);

    // Set the alarm
    if (rtc_set_alarm(&time, alarm_cb, arg) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    // Wait for the alarm to trigger
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}

void alarm_handler(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}