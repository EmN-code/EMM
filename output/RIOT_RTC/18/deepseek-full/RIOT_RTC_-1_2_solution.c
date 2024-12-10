
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"

#define ALARM_DELAY_SECONDS 5

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_init();

    struct tm time_struct = {
        .tm_year = 2024 - 1900, // years since 1900
        .tm_mon = 6 - 1,        // months since January (0-11)
        .tm_mday = 1,           // day of the month (1-31)
        .tm_hour = 0,           // hours since midnight (0-23)
        .tm_min = 0,            // minutes after the hour (0-59)
        .tm_sec = 0             // seconds after the minute (0-60)
    };

    rtc_set_time(&time_struct);

    struct tm alarm_time = time_struct;
    alarm_time.tm_sec += ALARM_DELAY_SECONDS;
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    while (1) {
        // Main loop to keep the program running
    }

    return 0;
}
