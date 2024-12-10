#include "ztimer.h"
#include "xtimer.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "sys/socket.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "esp_system.h"

void rtc_alarm_cb(void)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    rtc_init();

    struct tm time;
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 6 - 1;        // Month, where 0 = jan
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set an RTC alarm that triggers after 5 seconds
    struct tm alarm_time;
    memcpy(&alarm_time, &time, sizeof(struct tm));
    alarm_time.tm_sec += 5;

    // Adjust for minute overflow
    if (alarm_time.tm_sec >= 60) {
        alarm_time.tm_sec -= 60;
        alarm_time.tm_min += 1;
    }

    rtc_set_alarm(&alarm_time, rtc_alarm_cb);

    while (1) {
        // Keep the program running to wait for the alarm
        xtimer_sleep(10);
    }

    return 0;
}