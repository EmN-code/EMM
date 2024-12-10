#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "msg.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "dht.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "time_units.h"
#include "assert.h"
#include "mutex.h"

void rtc_callback(void)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Initializing RTC
    if (rtc_init(0) != 0) { // RTC_DEVICE 0 as an example
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Setting date and time to June 1, 2024 00:00:00
    struct tm time_info = {0};
    time_info.tm_year = 2024 - 1900;  // tm_year is years since 1900
    time_info.tm_mon = 6 - 1;         // tm_mon is months since January
    time_info.tm_mday = 1;
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;
    rtc_set_time(0, &time_info);

    // Adding an alarm 5 seconds later
    time_info.tm_sec += 5;
    rtc_set_alarm(0, &time_info, rtc_callback);

    // Waiting for the alarm indefinitely
    while(1) {
        thread_yield();
    }

    return 0;
}