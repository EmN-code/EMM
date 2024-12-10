#include "xtimer.h"
#include "ztimer.h"
#include "periph/cpuid.h"
#include "msg.h"

static ztimer_clock_t *_rtc = ZTIMER_MSEC;

void rtc_alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    // Assume the RTC starts counting from a base time
   
    struct tm time_struct = {
        .tm_year = 2024 - 1900,  // tm_year is year since 1900
        .tm_mon  = 6 - 1,        // tm_mon is month from 0 to 11
        .tm_mday = 1,            // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    time_t time = mktime(&time_struct); 

    ztimer_set_msg(_rtc, ztimer_now(_rtc) + 5000, rtc_alarm_callback, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}