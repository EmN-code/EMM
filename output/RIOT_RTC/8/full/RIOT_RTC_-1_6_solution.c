#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <kernel_defines.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <msg.h>
#include <ztimer.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <periph/gpio.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <mutex.h>

void mock_rtc_set_time(struct tm *time) {
    printf("[MOCK] RTC time set to: %02d-%02d-%04d %02d:%02d:%02d\n",
           time->tm_mday, time->tm_mon + 1, time->tm_year + 1900,
           time->tm_hour, time->tm_min, time->tm_sec);
}

void mock_rtc_set_alarm(struct tm *time, void (*cb)(void *), void *arg) {
    printf("[MOCK] RTC alarm set for: %02d-%02d-%04d %02d:%02d:%02d\n",
           time->tm_mday, time->tm_mon + 1, time->tm_year + 1900,
           time->tm_hour, time->tm_min, time->tm_sec);
    cb(arg);  // Directly call callback for demonstration purposes
}

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    struct tm alarm_time;
    time.tm_year = 2024 - 1900;
    time.tm_mon = 6 - 1;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    mock_rtc_set_time(&time);

    // Set alarm for 5 seconds later
    alarm_time = time;
    alarm_time.tm_sec += 5;

    while (1) {
        if (time.tm_sec >= 60) {
            alarm_time.tm_sec %= 60;
            alarm_time.tm_min += 1;
        }
        if (alarm_time.tm_min >= 60) {
            alarm_time.tm_min %= 60;
            alarm_time.tm_hour += 1;
        }

        mock_rtc_set_alarm(&alarm_time, alarm_callback, NULL);
        break;
    }

    return 0;
}