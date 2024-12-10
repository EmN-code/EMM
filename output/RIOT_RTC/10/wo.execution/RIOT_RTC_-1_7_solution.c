#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <mutex.h>
#include <unistd.h>
#include <timex.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <msg.h>
#include <mqtt/paho_mqtt.h>
#include <netemcute.h>
#include <net/sock/udp.h>
#include <sys/socket.h>
#include <net/gcoap.h>
#include <event/thread.h>
#include <mbox.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <kernel_defines.h>
#include <time_units.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>

void alarm_callback(void *arg) {
    (void)arg; // suppress unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize necessary variables and structures for the RTC
    struct tm timeinfo;
    ztimer_t alarm_timer;

    // Set date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // tm_year is years since 1900
    timeinfo.tm_mon = 6 - 1;        // tm_mon is 0-11
    timeinfo.tm_mday = 1;           // day of the month 1-31
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Set an alarm to trigger after 5 seconds
    alarm_timer.callback = alarm_callback;
    alarm_timer.arg = NULL;

    // Configure the timer to fire 5 seconds from now
    ztimer_set(ZTIMER_MSEC, &alarm_timer, 5000);

    // Let the main thread sleep while other threads execute
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}