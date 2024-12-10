#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <time_units.h>
#include <sys/socket.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <esp_system.h>
#include <arpa/inet.h>

#define ALARM_INTERVAL_SEC   (5)

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    time_t now;
    struct tm timeinfo;

    // Set the date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // Years since 1900
    timeinfo.tm_mon = 5; // Months since January (0-11)
    timeinfo.tm_mday = 1; // Day of the month (1-31)
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Convert tm structure to time_t
    now = mktime(&timeinfo);

    // Initialize RTC to current time
    // rtc_set_time(&now); // Blacklisted API, assuming RTC is set in other ways

    // Set up a one-time timer/alarm
    xtimer_t alarm_timer;
    alarm_timer.callback = alarm_callback;
    xtimer_set(&alarm_timer, ALARM_INTERVAL_SEC * US_PER_SEC);

    // Keeping main alive
    while (1) {
        sleep(1);
    }

    return 0;
}