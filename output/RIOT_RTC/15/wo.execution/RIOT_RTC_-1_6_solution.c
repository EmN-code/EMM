/* Description: This code implements an RTC alarm on an ESP32 using the RIOT operating system. The alarm is configured to trigger after 5 seconds, and upon activation, it prints "Alarm triggered!". */

#include "ztimer.h"
#include "thread.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "event/callback.h"
#include "event/periodic.h"
#include "event/thread.h"
#include "timex.h"
#include "time_units.h"
#include "xtimer.h"
#include "msg.h"
#include "unistd.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "stdatomic.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "periph/i2c.h"
#include "periph_cpu.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mbox.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_init();
    
    // Set the initial time to June 1st, 2024, 00:00:00
    struct tm time_info = {
        .tm_year = 2024 - 1900,  // Year since 1900
        .tm_mon = 5,             // Month (0-11, 0 = January)
        .tm_mday = 1,            // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    time_t timestamp = mktime(&time_info);
    rtc_set_time(timestamp);

    // Set alarm to trigger 5 seconds later
    struct tm alarm_info;
    rtc_get_time(&timestamp);
    gmtime_r(&timestamp, &alarm_info);
    alarm_info.tm_sec += 5;
    timestamp = mktime(&alarm_info);
    rtc_set_alarm(timestamp, alarm_callback, NULL);

    // Sleep to allow alarm to trigger
    xtimer_sleep(10);

    return 0;
}