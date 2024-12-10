#include <thread.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <MQTTClient.h>
#include <event/thread.h>
#include <event/callback.h>

void alarm_trigger(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm current_time;

    // Set date to June 1, 2024
    current_time.tm_year = 2024 - 1900; // Year since 1900
    current_time.tm_mon = 5;            // 0-11, so 5 is June
    current_time.tm_mday = 1;           // First day of month

    if (rtc_set_time(NULL, &current_time) != 0) {
        printf("Failed to set RTC time.\n");
        return -1;
    }

    printf("Time set to June 1, 2024\n");

    // Simulate waiting for 5 seconds for alarm to trigger
    struct tm alarm_time = current_time;
    alarm_time.tm_sec += 5;

    while (1) {
        struct tm now;
        rtc_get_time(NULL, &now);

        // Check if current time has reached or surpassed the alarm time
        if (difftime(mktime(&now), mktime(&alarm_time)) >= 0) {
            alarm_trigger();
            break;
        }
    }

    return 0;
}