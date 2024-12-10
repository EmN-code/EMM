#include <ztimer.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <periph_cpu.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mbox.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <event/callback.h>
#include <event/periodic.h>
#include <msg.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <arpa/inet.h>
#include <kernel_defines.h>

void alarm_callback(void *arg) {
    (void)arg; // Suppress unused variable warning
    puts("Alarm triggered!");
}

int main(void) {
    struct tm timeinfo;

    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 6 - 1; // Month, zero-based for struct tm
    timeinfo.tm_mday = 1; // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    timeinfo.tm_isdst = -1; // Not considering daylight saving time

    rtc_init();

    rtc_set_time(mktime(&timeinfo));

    printf("Time set to: %04d-%02d-%02d %02d:%02d:%02d\n",
           timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
           timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

    // Create alarm time (5 seconds later)
    time_t alarm_time = mktime(&timeinfo) + 5;

    // Setup alarm
    rtc_set_alarm(alarm_time, alarm_callback, NULL);

    // Sleep while waiting for alarm
    while (1) {
        xtimer_sleep(10); // Just sleep to save energy while idle
    }

    return 0;
}