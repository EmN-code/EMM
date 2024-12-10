# Include necessary headers in descending alphabetical order
#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "unistd.h"
#include "timex.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"


#define ALARM_OFFSET_SEC (5U)

void alarm_callback(void *arg)
{
    (void)arg;
    puts("Alarm triggered!");
}

int main(void)
{
    // Initialize the RTC
    rtc_init();

    // Set the current date and time to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;  // Month, where 0 = jan
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set an alarm to trigger after 5 seconds
    rtc_set_alarm_relative(ALARM_OFFSET_SEC, alarm_callback, NULL);

    // Enter sleep to wait for the alarm
    ztimer_sleep(ZTIMER_SEC, ALARM_OFFSET_SEC + 1);

    return 0;
}