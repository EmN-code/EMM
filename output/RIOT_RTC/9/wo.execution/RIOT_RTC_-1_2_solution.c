#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>

// Mock function to simulate RTC initialization
void rtc_init() {
    printf("RTC initialized.\n");
}

// Mock function to simulate current time setting
void rtc_set_current_time(int year, int month, int day, int hour, int minute, int second) {
    printf("Current time set to: %04d-%02d-%02d %02d:%02d:%02d\n", year, month, day, hour, minute, second);
}

// Mock function to simulate alarm setting
void rtc_set_alarm_in_seconds(int seconds_from_now, void (*callback_func)(void)) {
    printf("Alarm set for %d seconds from now.\n", seconds_from_now);
    sleep(seconds_from_now);
    callback_func();
}

// Callback function that gets called when alarm is triggered
void alarm_triggered_callback() {
    printf("Alarm triggered!\n");
}

int main() {
    rtc_init();
    rtc_set_current_time(2024, 6, 1, 0, 0, 0);  // Set date to June 1, 2024
    rtc_set_alarm_in_seconds(5, alarm_triggered_callback);  // Set alarm for 5 seconds later
    return 0;
}