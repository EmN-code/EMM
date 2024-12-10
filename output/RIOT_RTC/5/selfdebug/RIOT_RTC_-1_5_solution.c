#include <stdio.h>  // Necessary for printf function
#include <time.h>   // Necessary for struct tm

/* Callback function to be called when the alarm triggers */
void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main() {
    struct tm t;
    rtc_alarm_cb_t cb;
    
    // Set the date to June 1, 2024
    t.tm_year = 2024 - 1900;  // Year since 1900
    t.tm_mon = 5;             // Month, where 0 = January
    t.tm_mday = 1;            // Day of the month
    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 0;

    // Set the RTC time
    rtc_set_time(&t);

    // Set alarm to trigger after 5 seconds
    // Cloning the current time structure and adding 5 seconds
    struct tm alarm_time = t;
    alarm_time.tm_sec += 5;
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    while (1) {
        // Putting the main function into an idle loop to allow the callback to be triggered
        ; // Using a semicolon to indicate an empty statement for the loop
    }

    return 0;
}