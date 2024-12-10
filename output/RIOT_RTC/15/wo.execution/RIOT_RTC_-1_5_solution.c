#include <ztimer.h>
#include <unistd.h>
#include <thread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <event/thread.h>
#include <event/periodic.h>
#include <event/callback.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <timex.h>
#include <time_units.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <periph_cpu.h>
#include <periph/gpio.h>
#include <mbox.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <kernel_defines.h>

static void rtc_alarm_handler(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_set;
    time_set.tm_year = 2024 - 1900; // Year since 1900
    time_set.tm_mon = 6 - 1;        // Month, where 0 = January
    time_set.tm_mday = 1;           // Day of the month
    time_set.tm_hour = 0;
    time_set.tm_min = 0;
    time_set.tm_sec = 0;
    time_set.tm_isdst = -1;         // Not setting DST

    rtc_init();
    rtc_set_time(NULL, &time_set);

    struct tm alarm_time = time_set;
    alarm_time.tm_sec += 5; // Set alarm for 5 seconds later

    rtc_set_alarm(NULL, &alarm_time, rtc_alarm_handler, NULL);

    // Enter sleep to simulate waiting for the alarm
    ztimer_sleep(ZTIMER_SEC, 10);
    
    return 0;
}