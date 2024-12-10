#include "xtimer.h"
#include "stdio.h"
#include "periph_rtc.h"
#include "periph_timer.h"

void alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm alarm_time;
    rtc_alarm_cb_t callback = alarm_callback;

    // Set the initial date to June 1, 2024
    struct tm init_time = {
        .tm_year = 2024 - 1900, // Years since 1900
        .tm_mon = 5,  // Month, counting from 0 (June is month 5)
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    rtc_set_time(&init_time);

    // Configure the alarm to trigger after 5 seconds
    alarm_time = init_time;
    alarm_time.tm_sec += 5;

    while (1) {
        int result = rtc_set_alarm(&alarm_time, callback, NULL);
        // Check if alarm was set successfully
        if (result != 0) {
            printf("Setting alarm failed, retrying...\n");
            xtimer_sleep(1);  // Wait for one second before retrying
        } else {
            printf("Alarm set successfully.\n");
            break;
        }
    }

    // Main loop does nothing, waiting for the alarm
    while (1) {
        xtimer_sleep(1);  // Sleep to save power
    }

    return 0;
}