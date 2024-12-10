#include "periph/rtc.h"
#include "stdio.h"
#include "stdbool.h"
#include "sched.h"
#include "random.h"
#include "nanosleep.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900;  // Years since 1900
    time.tm_mon = 6 - 1;         // Month, where 0 = January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set the current date
    rtc_set_time(&time);

    // Prepare alarm time, 5 seconds from now
    time.tm_sec += 5;
    mktime(&time); // Normalize the time structure

    rtc_set_alarm(&time, alarm_callback, NULL);

    while (true) {
        // Main loop doing nothing, waiting for the alarm
        sleep(1);
    }
    return 0;
}