#include <stdio.h>
#include <time.h>
#include <esp_err.h>
#include <esp_types.h>

// Callback function that triggers when the RTC alarm goes off
void rtc_alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main() {
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900; // Years since 1900
    timeinfo.tm_mon = 5;            // Months since January (June)
    timeinfo.tm_mday = 1;           // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Initialize and set the RTC time
    if (rtc_set_time(&timeinfo) != ESP_OK) {
        printf("Failed to set RTC time\n");
        return -1;
    }

    // Get the current time (for demonstration purposes)
    rtc_get_time(&timeinfo);
    printf("Current time set to: %s", asctime(&timeinfo));

    // Set the alarm 5 seconds from now
    timeinfo.tm_sec += 5;
    void *arg = NULL; // Arguments can be passed to callback, if needed

    if (rtc_set_alarm(&timeinfo, rtc_alarm_callback, arg) != 0) {
        printf("Failed to set RTC alarm\n");
        return -1;
    }

    printf("Alarm is set for 5 seconds from now...\n");

    while (1) {
        // Normally, system enter low-power mode or keep running tasks
        // Waiting for alarm interrupt...
    }

    return 0;
}