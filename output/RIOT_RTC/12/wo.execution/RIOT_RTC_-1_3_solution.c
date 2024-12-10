#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <timex.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <periph/rtc.h>

void alarm_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm alarm_time;
    alarm_time.tm_year = 2024 - 1900;  // Years since 1900
    alarm_time.tm_mon = 5;             // June (0-based, so 5 means June)
    alarm_time.tm_mday = 1;            // 1st day of the month
    alarm_time.tm_hour = 0;
    alarm_time.tm_min = 0;
    alarm_time.tm_sec = 5;             // Alarm after 5 seconds

    rtc_init();
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Loop to keep the program running
    while (1) {
        sleep(1);
    }

    return 0;
}