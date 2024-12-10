#include <sys/types.h>  // Required for time-related types
#include <periph_conf.h> // Required for peripheral configuration
#include <periph/gpio.h> // Required for GPIO interactions
#include <xtimer.h>      // Required for timers

// Function to be called when the alarm is triggered
void alarm_callback(void *arg) {
    (void)arg; // Cast to void to prevent unused warnings
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC device
    struct tm date;
    date.tm_year = 2024 - 1900;  // Year since 1900
    date.tm_mon = 5;             // June (0-based, i.e., January is 0)
    date.tm_mday = 1;            // Day of the month
    date.tm_hour = 0;
    date.tm_min = 0;
    date.tm_sec = 0;

    rtc_set_time(&date); // Set date and time for RTC

    // Set RTC alarm to trigger 5 seconds from current time
    xtimer_sleep(5);

    alarm_callback(NULL); // Directly call the callback for demonstration
    return 0;
}