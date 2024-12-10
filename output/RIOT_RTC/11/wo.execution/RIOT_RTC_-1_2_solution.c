#include <unistd.h>
#include <time_units.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <periph/rtc.h>

void alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_time_t time = {
        .tm_year = 2024,
        .tm_mon = 6,
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    if (rtc_init() != 0) {
        puts("RTC initialization failed\n");
        return 1;
    }

    if (rtc_set_time(&time) != 0) {
        puts("Setting time failed\n");
        return 1;
    }

    /* Set an alarm to trigger in 5 seconds */
    rtc_set_alarm_reltimer(ALARM_IN_SECONDS(5), alarm_callback, NULL);

    /* Main loop */
    while (1) {
        /* Sleep and let the interrupt wake us on alarm */
        sleep(1);
    }

    return 0;
}

/* Note: The macro ALARM_IN_SECONDS is used here to calculate relative alarm time in seconds */