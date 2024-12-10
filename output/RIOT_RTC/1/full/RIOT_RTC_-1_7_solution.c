#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <periph/rtc.h>
#include <ztimer.h>
#include <assert.h>
#include <net/sock/util.h>
#include <paho_mqtt.h>
#include <net/sock/udp.h>
#include <msg.h>
#include <net/gcoap.h>
#include <MQTTClient.h>
#include <net/emcute.h>
#include <mutex.h>
#include <periph/i2c.h>
#include <time_units.h>
#include <periph/gpio.h>

void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_init();

    struct tm time;
    time.tm_year = 2024 - 1900;
    time.tm_mon = 6 - 1;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_time_set(&time);

    ztimer_sleep(ZTIMER_MSEC, 5000);

    rtc_alarm_set(5, rtc_alarm_callback, NULL);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}