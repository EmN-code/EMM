#include <xtimer.h>
#include <periph/rtc.h>
#include <ztimer.h>
#include <stdio.h>

#define ALARM_DELAY 5 // Set the alarm to trigger after 5 seconds

// Structure to hold simulated RTC time
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} simulated_rtc_time_t;

// Function to simulate incrementing RTC time
void increment_time(simulated_rtc_time_t *time) {
    time->second++;
    if (time->second >= 60) {
        time->second = 0;
        time->minute++;
    }
    if (time->minute >= 60) {
        time->minute = 0;
        time->hour++;
    }
    if (time->hour >= 24) {
        time->hour = 0;
        time->day++;
    }
    // Note: This is a simple increment logic, does not handle month or year overflow
}

int main(void) {
    simulated_rtc_time_t current_time = {2024, 6, 1, 0, 0, 0};
    int elapsed_seconds = 0;

    printf("Initial Date and Time: %d-%02d-%02d %02d:%02d:%02d\n", 
            current_time.year, current_time.month, current_time.day, 
            current_time.hour, current_time.minute, current_time.second);

    while (1) {
        xtimer_sleep(1); // Wait for 1 second
        increment_time(&current_time); // Simulate RTC increment
        elapsed_seconds++;

        printf("Current Simulated Time: %d-%02d-%02d %02d:%02d:%02d\n", 
                current_time.year, current_time.month, current_time.day, 
                current_time.hour, current_time.minute, current_time.second);

        if (elapsed_seconds >= ALARM_DELAY) {
            printf("Alarm triggered!\n");
            break;
        }
    }

    return 0;
}