#include "xtimer.h"
#include "ztest.h"
#include "kernel_defines.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "periph/rtc.h"
#include "esp_system.h"

static void set_rtc_alarm(void)
{
    struct tm time;

    /* Set current date: 2024-06-01 */
    time.tm_year = 2024 - 1900;
    time.tm_mon  = 5;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min  = 0;
    time.tm_sec  = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time\n");
    }

    /* Set alarm 5 seconds from now */
    if (rtc_set_alarm_relative(5) != 0) {
        printf("Failed to set alarm\n");
    }
    else {
        printf("Alarm set for 5 seconds\n");
    }
}

int main(void)
{
    printf("RTC Alarm Example\n");

    rtc_init();
    set_rtc_alarm();
    
    /* Start thread that waits for the alarm */
    while (1) {
        if (rtc_get_alarm() > time()) {
            printf("Alarm triggered!\n");
        }
        xtimer_sleep(1);
    }
    
    return 0;
}