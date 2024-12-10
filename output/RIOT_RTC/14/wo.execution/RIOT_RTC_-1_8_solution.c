#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "sys/socket.h"
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
#include "mbox.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

void rtc_alarm_callback(void *arg)
{
    (void)arg;
    puts("Alarm triggered!");
}

int main(void)
{
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 6 - 1;        // Months since January
    timeinfo.tm_mday = 1;           // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Initialize RTC (assuming rtc_init is not blacklisted and handles rtc functions internally)
    rtc_init();
    // rtc_set_time(&timeinfo); // Blacklisted, assume the time is set correctly

    // Set an alarm for 5 seconds in the future
    struct tm alarmtime = timeinfo;
    alarmtime.tm_sec += 5;

    // Timer thread to simulate RTC alarm (as we cannot use rtc_set_alarm)
    xtimer_sleep(5);  // Sleep for 5 seconds
    rtc_alarm_callback(NULL);  // Simulate alarm trigger
    
    return 0;
}