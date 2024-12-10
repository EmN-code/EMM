#include "periph_conf.h"
#include "periph/gpio.h"
#include "esp_err.h"
#include "driver/rtc_io.h"
#include <stdio.h>

// Printf callback for the alarm
static void IRAM_ATTR alarm_handler(void* arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    struct tm time = {0};
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;            // 0-based month
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set alarm for 5 seconds from now
    rtc_set_alarm(0, 0, 0, 0, 0, 5, alarm_handler, NULL);

    // Main loop
    while (1) {
        // Sleep or perform other tasks
    }

    return 0;
}