#include <ztimer.h>
#include <stdio.h>
#include <inttypes.h>
#include <time.h>  // make sure this is supported by the RIOT environment according to your specs.

// Function prototypes
void rtc_set_time(struct tm *time_info);
void rtc_get_time(struct tm *time_info);
void rtc_set_alarm(struct tm *alarm_time);

void rtc_set_time(struct tm *time_info) {
    // Set the RTC time with provided time_info
    // This might usually interface with a hardware RTC module
}

void rtc_get_time(struct tm *time_info) {
    // Get the current time from the RTC
    // This would usually involve reading from the hardware RTC module
}

void rtc_set_alarm(struct tm *alarm_time) {
    // Set an RTC alarm
    // This typically sets the alarm time on a hardware RTC or a software-implemented timer
}

void alarm_callback(void *arg) {
    (void)arg;  // suppress unused variable warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_info;

    // Set date to June 1, 2024
    time_info.tm_year = 2024 - 1900;
    time_info.tm_mon = 5;
    time_info.tm_mday = 1;
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;

    rtc_set_time(&time_info);

    // Set alarm for 5 seconds later
    rtc_get_time(&time_info);
    time_info.tm_sec += 5;  // Add 5 seconds
    mktime(&time_info);  // Normalize the structure
    rtc_set_alarm(&time_info);

    return 0;
}