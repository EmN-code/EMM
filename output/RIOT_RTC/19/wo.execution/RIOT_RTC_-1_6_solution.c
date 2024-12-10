#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "xtimer.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "thread.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "sys/types.h" // Assuming general inclusion to help define missing types

static void rtc_alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();

    // Set current time to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900;
    time.tm_mon = 5;  // June, as months are 0-11
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set RTC time (applying a mock or replacement function)
    rtc_set_time(&time);

    // Alarm setup for 5 seconds after set time
    rtc_alarm_t alarm;
    alarm.time.tm_sec = time.tm_sec + 5;
    alarm.callback = rtc_alarm_callback;

    // Set RTC alarm (applying a mock or replacement function)
    rtc_set_alarm(&alarm);

    // Put the thread to sleep to allow time for alarm triggering
    while (1) {
        thread_sleep();
    }
    return 0;
}