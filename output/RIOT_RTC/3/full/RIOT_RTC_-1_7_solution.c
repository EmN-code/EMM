#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <string.h>
#include <rtc.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mutex.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <dht_params.h>
#include <dht.h>
#include <assert.h>
#include <arpa/inet.h>
#include <MQTTClient.h>

void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_details;
    rtc_set_t rtc_time;

    time_details.tm_year = 2024 - 1900;  // Year since 1900
    time_details.tm_mon = 5;             // Months since January [0-11]
    time_details.tm_mday = 1;            // Day of the month [1-31]
    time_details.tm_hour = 0;
    time_details.tm_min = 0;
    time_details.tm_sec = 0;

    rtc_time.tm_sec = mktime(&time_details);

    rtc_init();

    if(rtc_set_time(&rtc_time) != 0) {
        printf("RTC setting failed!\n");
        return 1;
    }

    if(rtc_set_alarm(rtc_alarm_cb, rtc_time + 5) != 0) {
        printf("RTC alarm setting failed!\n");
        return 1;
    }

    while(true) {
        thread_yield();
    }

    return 0;
}