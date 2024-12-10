#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <thread.h>
#include <net/emcute.h>
#include <msg.h>
#include <timex.h>
#include <sys/socket.h>
#include <evtimer_msg.h>
#include <periph/rtc.h>  // Required for RTC functions
#include <mutex.h>

// Callback function that gets triggered by the RTC alarm
void alarm_callback(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    // Set the RTC time to June 1, 2024
    struct tm time = {0};
    time.tm_year = 2024 - 1900;  // Years since 1900
    time.tm_mon = 6 - 1;         // June (0-based month)
    time.tm_mday = 1;

    // Convert to time_t and set RTC
    time_t unix_time = mktime(&time);
    rtc_set_time((struct tm*)&unix_time);

    // Set the alarm for 5 seconds from now
    rtc_alarm_cb_t alarm = alarm_callback;  // Correct type usage
    struct tm alarm_time;
    rtc_get_time(&alarm_time);  // Get the current time
    alarm_time.tm_sec += 5;     // Alarm 5 seconds later
    if (alarm_time.tm_sec >= 60) {
        alarm_time.tm_sec -= 60;
        alarm_time.tm_min += 1;
    }

    rtc_alarm_t alarm_config;   // Correct struct usage
    alarm_config.time = alarm_time;
    alarm_config.cb = alarm_callback;
    alarm_config.arg = NULL;

    // Set RTC alarm
    rtc_set_alarm(&alarm_config);

    // Keep the main thread active to wait for the alarm
    while (1) {
        thread_sleep();  // Induce sleep to save resources
    }

    return 0;
}