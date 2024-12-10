#include <stdio.h>
#include <driver/rtc_io.h>
#include <driver/rtc_slow.h>
#include <driver/rtc.h>
#include <esp32/rom/rtc.h>

void rtc_callback(void)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct timeval alarm_time;
    gettimeofday(&alarm_time, NULL);
    alarm_time.tv_sec += 5; // Set alarm to trigger after 5 seconds

    struct tm timeinfo = {
        .tm_year = 2024 - 1900, // Years since 1900
        .tm_mon = 5, // Months since January (0-11)
        .tm_mday = 1, // Day of the month (1-31)
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    time_t future_date = mktime(&timeinfo);
    struct timeval set_time = { .tv_sec = future_date, .tv_usec = 0 };

    settimeofday(&set_time, NULL);
    printf("Current date set to: %s", asctime(&timeinfo));

    // Enable the RTC alarm
    rtc_set_time(RTC_SLOW_CLK_FREQ_32K * future_date);
    rtc_set_alarm(RTC_SLOW_CLK_FREQ_32K * alarm_time.tv_sec + RTC_SLOW_CLK_FREQ_32K * alarm_time.tv_usec / 1000000);
    rtc_init();

    return 0;
}