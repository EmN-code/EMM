#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include <stdio.h>

void delay(int milliseconds) {
    volatile int count;
    while (milliseconds-- > 0) {
        count = 32000; // Busy-wait loop
        while (count-- > 0);
    }
}

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} custom_rtc_time_t;

int set_custom_rtc_time(custom_rtc_time_t time) {
    // Pseudo-function to implement setting of RTC time
    printf("RTC time set to %d-%02d-%02d %02d:%02d:%02d\n", time.year, time.month, time.day, time.hour, time.minute, time.second);
    return 0;  // Assuming setting time always succeeds
}

void rtc_alarm_callback(void) {
    puts("Alarm triggered!");
}

int main() {
    custom_rtc_time_t rtc_time = {2024, 6, 1, 0, 0, 0};
    int retry_count = 3;
    
    while (retry_count-- > 0) {
        if (set_custom_rtc_time(rtc_time) == 0) {
            puts("RTC time successfully set.");
            break;
        } else {
            puts("Failed to set RTC time, retrying...");
            delay(1000); // Delay 1 second
        }
    }

    delay(5000); // Simple delay to simulate alarm time wait
    rtc_alarm_callback();

    return 0;
}