#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "string.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "arpa/inet.h"
#include "time_units.h"
#include "dht_params.h"

void alarm_callback(void)
{
    puts("Alarm triggered!");
}

int main(void)
{
    /* Initialize RTC */
    if (rtc_init() != 0) {
        puts("RTC initialization failed.");
        return -1;
    }

    /* Set the RTC to June 1, 2024, 00:00:00 */
    struct tm time;
    time.tm_year = 2024 - 1900;
    time.tm_mon = 5;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&time);

    /* Set the alarm to trigger in 5 seconds */
    struct tm alarm_time;
    rtc_get_time(&alarm_time);
    alarm_time.tm_sec += 5;
    mktime(&alarm_time); // Normalize the time structure

    rtc_set_alarm(&alarm_time, alarm_callback);

    while (1) {
        /* Sleep to save power and wait for the alarm callback */
        xtimer_usleep(6000000); // Sleep for 6 seconds
    }

    return 0;
}