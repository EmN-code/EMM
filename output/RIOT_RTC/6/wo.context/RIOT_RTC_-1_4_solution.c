#include "xtimer.h"
#include "stdio.h"
#include "time.h"
#include "periph/rtc.h"

void rtc_alarm_callback(void *arg) {
    (void)arg; // unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    rtc_set_time(epoch_to_tm(1717248000, &timeinfo));  // Set date to June 1, 2024

    struct tm alarminfo = timeinfo;
    alarminfo.tm_sec += 5;  // Set alarm 5 seconds after current time

    if (rtc_set_alarm(&alarminfo, rtc_alarm_callback, NULL) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    printf("RTC set to June 1, 2024, alarm in 5 seconds\n");

    // Busy loop to keep the program running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}