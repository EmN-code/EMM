#include <stdio.h>
#include <time.h>
#include <sys/types.h>

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main() {
    struct tm timeinfo;
    time(&timeinfo);
    printf("Current time: %s\n", asctime(&timeinfo));

    // Set date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // Years since 1900
    timeinfo.tm_mon = 6 - 1;        // Month, where 0 = jan
    timeinfo.tm_mday = 1;           // Day of the month

    // Set hardware RTC to the specified time
    rtc_set_time(&timeinfo);

    // Set alarm for 5 seconds from the new time
    timeinfo.tm_sec += 5;

    // Register the alarm callback
    rtc_set_alarm(&timeinfo, alarm_callback, NULL);

    return 0;
}