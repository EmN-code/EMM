#include <ztimer.h>
#include <string.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <pthread.h>
#include <sys/socket.h>
#include <mqtt_transport_socket.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <timex.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <msg.h>
#include <mbox.h>
#include <mutex.h>
#include <xtimer.h>
#include <thread.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <event/callback.h>

#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"

static void *alarm_thread(void *arg);

static bool alarm_triggered = false;

static void set_rtc(void) {
    struct tm time = {
        .tm_year = 124, // Years since 1900
        .tm_mon = 5,    // 0-based
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    rtc_set_time(&time);
}

static void set_alarm(void) {
    struct tm alarm_time = rtc_get_time();
    alarm_time.tm_sec += 5;

    rtc_set_alarm(&alarm_time);

    rtc_get_desc_mbox(alarm_trigger, NULL);  // Assuming an available library function for this
}

static void alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
    alarm_triggered = true;
}

static void *alarm_thread(void *arg) {
    (void)arg;
    while (!alarm_triggered) {
        // Sleep this thread till the alarm has been triggered
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    puts("Initializing RTC with Alarm functionality\n");
    rtc_init();

    set_rtc();
    puts("RTC time set to June 1, 2024\n");

    set_alarm();
    puts("Alarm set for 5 seconds from now\n");

    pthread_t alarm_thr;
    pthread_create(&alarm_thr, NULL, alarm_thread, NULL);
    pthread_join(alarm_thr, NULL);

    return 0;
}