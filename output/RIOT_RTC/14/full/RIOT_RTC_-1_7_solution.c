
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "timex.h"
#include "ztimer.h"
#include "unistd.h"
#include "uart.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "event/callback.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "xtimer.h"
#include "time_units.h"

void alarm_handler(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_init();

    struct tm time = {0};
    time.tm_year = 124;  // years since 1900
    time.tm_mon = 5;    // June
    time.tm_mday = 1;   // 1st
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;  // add 5 seconds for the alarm
    rtc_set_alarm(&alarm_time, alarm_handler);

    while (1) {
        // Keep this loop to hold the program for any kind of additional processing
        sleep(1);
    }
    return 0;
}