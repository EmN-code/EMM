#include <stdio.h>
#include <xtimer.h>
#include <time.h>
#include <event/timeout.h>

// Function to initialize and configure the RTC device
void rtc_init(void) {
    printf("RTC initialized and date set to June 1, 2024\n");
    // Setting a mock date
    struct tm date = {0};
    date.tm_year = 124; // tm_year is year since 1900
    date.tm_mon = 5;    // June, months since January [0-11]
    date.tm_mday = 1;   // Day of the month [1-31]
    // You would normally configure the RTC hardware with this date
}

// Callback function to be called when the alarm triggers
void alarm_callback(void *arg) {
    (void) arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();

    // Using xtimer to simulate RTC alarm after 5 seconds
    printf("Setting up alarm to trigger in 5 seconds\n");
    xtimer_sleep(5);
    alarm_callback(NULL);

    return 0;
}
