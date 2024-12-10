#include "ztimer.h"
#include "stdatomic.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "esp_system.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "stdbool.h"
#include "stddef.h"
#include "string.h"
#include "time_units.h"
#include "net/emcute.h"
#include "unistd.h"
#include "net/sock/udp.h"
#include "stdlib.h"
#include "evtimer_msg.h"
#include "mutex.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "time.h"
#include "net/gcoap.h"
#include "esp_wifi.h"
#include "msg.h"
#include "xtimer.h"
#include "mbox.h"
#include "sys/socket.h"
#include "arpa/inet.h"

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    /* Initialize the RTC */
    if (rtc_init() != 0) {
        puts("RTC initialization failed");
        return 1;
    }

    /* Set the date to June 1, 2024 at 00:00:00 */
    struct tm time = {
        .tm_year = 2024 - 1900, // years since 1900
        .tm_mon = 5,            // months since January [0-11]
        .tm_mday = 1,           // day of the month [1-31]
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    if (rtc_set_time(&time) != 0) {
        puts("Setting RTC time failed");
        return 1;
    }

    /* Set an alarm for 5 seconds later */
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    if (rtc_set_alarm(&alarm_time, alarm_callback, NULL) != 0) {
        puts("Setting RTC alarm failed");
        return 1;
    }

    /* Put the MCU to sleep to wait for the alarm */
    ztimer_sleep(ZTIMER_MSEC, 6000); // sleep for more than 5 sec to ensure alarm triggers

    return 0;
}
