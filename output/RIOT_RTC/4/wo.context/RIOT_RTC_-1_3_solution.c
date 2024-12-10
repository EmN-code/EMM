#include <stdio.h>
#include <time.h>
#include "event/periodic.h"

// Callback function for the alarm
void alarm_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    time_t now;
    struct tm new_time;

    // Set the date to June 1, 2024
    new_time.tm_year = 2024 - 1900;
    new_time.tm_mon = 6 - 1;
    new_time.tm_mday = 1;
    new_time.tm_hour = 0;
    new_time.tm_min = 0;
    new_time.tm_sec = 0;
    new_time.tm_isdst = -1; // Daylight saving time flag

    // Convert to time_t
    now = mktime(&new_time);

    // Set the system time
    if (now != (time_t)-1) {
        struct timespec ts = {.tv_sec = now, .tv_nsec = 0};
        clock_settime(CLOCK_REALTIME, &ts);
    }

    // Define and initialize the periodic event
    event_periodic_callback_t event;
    ztimer_clock_t *clock = ZTIMER_MSEC;
    event_queue_t queue;

    // Create the periodic callback event
    event_periodic_callback_create(&event, clock, 5000, &queue, alarm_callback);

    printf("System initialized. Waiting for alarm to trigger...\n");

    // Main loop
    while (1) {
        // Process the events
        event_periodic_callback_get_count(&event);
    }

    return 0;
}