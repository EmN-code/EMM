#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "xtimer.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "mbox.h"
#include "msg.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 6 - 1;        // Months since January
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_init(); // Initialize RTC
    rtc_set_time(&time); // Set the time to June 1, 2024

    uint32_t alarm_seconds = 5; // Alarm after 5 seconds
    rtc_set_alarm_relative(alarm_seconds, alarm_callback, NULL);

    // Main loop
    while (1) {
        xtimer_sleep(1); // Sleep to reduce CPU use
    }
    return 0;
}
