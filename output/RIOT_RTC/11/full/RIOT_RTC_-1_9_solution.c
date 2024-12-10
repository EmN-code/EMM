#include "xtimer.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "periph/rtc.h"
#include "stdint.h"
#include "timex.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "keeper.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "sys/socket.h"
#include "unistd.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "msg.h"
#include "mutex.h"

#define ALARM_DELAY 5

static void alarm_handler(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm alarm_time;
    rtc_init();

    // Set the RTC time to June 1, 2024
    alarm_time.tm_year = 2024 - 1900; // year since 1900
    alarm_time.tm_mon = 6 - 1;        // months since January
    alarm_time.tm_mday = 1;           // day of the month
    alarm_time.tm_hour = 0;
    alarm_time.tm_min = 0;
    alarm_time.tm_sec = 0;

    if (rtc_set_time(&alarm_time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set up the alarm to trigger in 5 seconds
    alarm_time.tm_sec += ALARM_DELAY;
    mktime(&alarm_time); // normalize the time structure

    if (rtc_set_alarm(&alarm_time, alarm_handler, NULL) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    while (1) {
        // Sleep to let the alarm trigger
        xtimer_sleep(10);
    }

    return 0;
}