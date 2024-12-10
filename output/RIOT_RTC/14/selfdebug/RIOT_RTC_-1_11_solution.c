#include <xtimer.h>
#include <stdio.h>
#include <rtc.h>

void alarm_callback(void *arg) {
    (void)arg;  // Use (void)arg to silence unused variable warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 6 - 1;        // Months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set system time to January 1, 2024
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time\n");
        return 1;
    }

    printf("System time set\n");

    // Trigger alarm 5 seconds later
    time.tm_sec += 5;

    if (rtc_set_alarm(&time, alarm_callback, NULL) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    printf("Alarm set for 5 seconds later\n");

    // Keeping main alive to wait for the alarm
    xtimer_sleep(10);

    return 0;
}