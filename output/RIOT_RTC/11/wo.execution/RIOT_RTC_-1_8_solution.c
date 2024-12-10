#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "ztimer.h"
#include "timex.h"
#include "msg.h"
#include "xtimer.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "esp_system.h"
#include "unistd.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "mbox.h"

void alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("RTC Alarm example on ESP32\n");
    rtc_init();

    // Configure RTC time to June 1, 2024, 00:00:05
    struct tm time;
    time.tm_year = 2024 - 1900;  // Years since 1900
    time.tm_mon = 6 - 1;         // Months since January
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&time);

    ztimer_sleep(ZTIMER_MSEC, 5000);  // Wait for 5 seconds to simulate the alarm time

    // Set alarm
    rtc_set_alarm_cb(alarm_callback);
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    // Wait until alarm is triggered (simulation)
    xtimer_sleep(5);

    return 0;
}
