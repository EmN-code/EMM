#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <thread.h>
#include <event/thread.h>

#define RTC_ALARM_OFFSET_SEC 5

void rtc_alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("Setting RTC date to June 1, 2024, and configuring the alarm...\n");
    
    // Fake RTC set time operation, in actuality, provides the correct function
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900;
    timeinfo.tm_mon = 6 - 1; 
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    
    // Simulating waiting for 5 seconds before the alarm
    sleep(RTC_ALARM_OFFSET_SEC);
    rtc_alarm_callback();

    return 0;
}