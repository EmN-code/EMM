#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "string.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "dht_params.h"
#include "arpa/inet.h"

#define ALARM_DURATION (5ULL * US_PER_SEC)  // Alarm duration set to 5 seconds

static void rtc_alarm_callback(void)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    // RTC configuration
    struct tm time;
    rtc_get_time(&time); 

    // Set the RTC time to June 1, 2024
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 5; // 0-based, so 5 = June
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set up an RTC alarm to trigger in 5 seconds
    rtc_alarm_t alarm;
    alarm.time = rtc_get_time() + ALARM_DURATION;
    alarm.callback = rtc_alarm_callback;
    rtc_set_alarm(&alarm);
    
    // Simulate waiting for the alarm
    while (1) {
        __WFI(); // Wait for interrupt
    }

    return 0;
}
