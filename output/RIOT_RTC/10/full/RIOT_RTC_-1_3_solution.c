#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "thread.h"
#include "xtimer.h"

#define SECONDS_TO_MICROSECONDS 1000000

typedef struct {
    int tm_year;
    int tm_mon;
    int tm_mday;
    int tm_hour;
    int tm_min;
    int tm_sec;
} simple_rtc_time_t;

void alarm_callback(void)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    printf("Initializing RTC for ESP32\n");

    simple_rtc_time_t current_time = { .tm_year = 2024, .tm_mon = 6, .tm_mday = 1, .tm_hour = 0, .tm_min = 0, .tm_sec = 0 };    

    // Simulate setting the RTC
    printf("Setting initial time: 2024-06-01 00:00:00\n");

    // Simulate scheduling the alarm after 5 seconds using a busy-wait
    printf("Scheduling alarm in 5 seconds\n");
    xtimer_usleep(5 * SECONDS_TO_MICROSECONDS);
    alarm_callback();

    return 0;
}