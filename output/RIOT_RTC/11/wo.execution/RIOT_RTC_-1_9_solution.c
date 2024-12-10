// Description:
// This code sets up an RTC on an ESP32 using the RIOT operating system. We configure the RTC to set the current date
to June 1, 2024, and an alarm is set to trigger after 5 seconds. Upon the alarm triggering, it prints "Alarm triggered!".
//
// Includes are organized alphabetically in descending order as per the requirement.

#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "stddef.h"
#include "stdlib.h"
#include "arpa/inet.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "net/emcute.h"

void alarm_callback(void *arg) {
    (void)arg; // unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    rtc_init();

    // Set the date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 6 - 1;         // Month, 0 - jan
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&time);

    // Set an alarm to trigger after 5 seconds
    rtc_set_alarm_reltimer(5);

    // Set the alarm callback
    rtc_set_alarm(alarm_callback, NULL);

    // Sleep to keep the program running
    ztimer_sleep(ZTIMER_MSEC, 10000);

    return 0;
}