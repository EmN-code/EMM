#include <stdio.h>  // Standard input-output library for printing
#include <time.h>   // Time library for struct tm and time functions

// Function to be called when the alarm is triggered
void alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main() {
    // Initialize and set the date to June 1, 2024
    struct tm alarm_time;
    alarm_time.tm_year = 2024 - 1900; // tm_year is the year since 1900
    alarm_time.tm_mon = 6 - 1;        // tm_mon is the month from 0 to 11
    alarm_time.tm_mday = 1;
    alarm_time.tm_hour = 0;
    alarm_time.tm_min = 0;
    alarm_time.tm_sec = 0;
    alarm_time.tm_isdst = -1;         // Is daylight saving time in effect? -1 (not set)

    time_t current_time, target_time;
    time(&current_time);              // Get the current time
    target_time = mktime(&alarm_time);

    // Calculate time difference to sleep
    double seconds = difftime(target_time, current_time);

    // Add delay of 5 additional seconds for the alarm trigger
    seconds += 5;
    printf("Waiting for alarm...\n");

    // Simulate waiting for the alarm by sleeping
    struct timespec ts;
    ts.tv_sec = (int)seconds;
    ts.tv_nsec = (seconds - ts.tv_sec) * 1e9;
    nanosleep(&ts, NULL);

    // Trigger the alarm callback
    alarm_callback();

    return 0;
}