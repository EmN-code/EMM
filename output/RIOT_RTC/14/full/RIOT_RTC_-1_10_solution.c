#include "xtimer.h"
#include "ztimer.h"
#include "stdio.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "esp_wifi.h"
#include "sys/socket.h"
#include "esp_system.h"
#include "netinet/in.h"
#include "string.h"
#include "stddef.h"
#include "esp_system.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "event/callback.h"
#include "event/thread.h"
#include "stdbool.h"
#include "timex.h"
#include "mbox.h"
#include "time_units.h"
#include "thread.h"
#include "kernel_defines.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mutex.h"

void rtc_alarm_callback(void *arg)
{
    // This function will be called when the RTC alarm is triggered
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm time;

    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900; // tm_year is years since 1900
    time.tm_mon = 5;            // tm_mon is months since January (0-11)
    time.tm_mday = 1;           // tm_mday is day of the month (1-31)
    time.tm_hour = 0;           // tm_hour is hours since midnight (0-23)
    time.tm_min = 0;            // tm_min is minutes after the hour (0-59)
    time.tm_sec = 0;            // tm_sec is seconds after the minute (0-60)

    rtc_init();
    rtc_set_time(&time);

    // Set the alarm to trigger 5 seconds from the current time
    time.tm_sec += 5;

    rtc_set_alarm(&time, rtc_alarm_callback, NULL);

    // Keep the program running indefinitely so it can wait for the alarm
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}