#include <xtimer.h>
#include <ztimer.h>
#include <unistd.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stddef.h>
#include <paho_mqtt.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <MQTTClient.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    rtc_init();

    // Set the date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // years since 1900
    time.tm_mon = 6 - 1; // months since January
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0; // Hours
    time.tm_min = 0; // Minutes
    time.tm_sec = 0; // Seconds
    time.tm_isdst = -1; // No daylight saving time
    rtc_set_time(&time);

    // Set an alarm for 5 seconds in the future
    time.tm_sec += 5;

    // Register alarm callback
    int result = rtc_set_alarm(&time, alarm_callback, NULL);
    if (result != 0) {
        printf("Failed to set alarm.\n");
        return 1;
    }

    // Let the program wait while the alarm is pending
    while (1) {
        sleep(1);
    }

    return 0;
}