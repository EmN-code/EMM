#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <netinet/in.h>
#include <mbox.h>
#include <msg.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>

void rtc_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

void set_rtc_alarm(void) {
    struct tm time;
    rtc_get_time(&time);
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 5;            // June
    time.tm_mday = 1;           // 1st
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);
    
    // Set an alarm 5 seconds after the set time
    ztimer_sleep(ZTIMER_SEC, 5);

    if (rtc_set_alarm(&time, rtc_callback, NULL) != 0) {
        printf("Failed to set RTC alarm!\n");
    }
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed!\n");
        return 1;
    }

    // Set RTC time and alarm
    set_rtc_alarm();

    // Do nothing, wait for alarm
    while (1) {
        pause(); 
    }

    return 0;
}