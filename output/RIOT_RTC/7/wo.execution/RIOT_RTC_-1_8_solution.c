#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "stddef.h"
#include "esp_system.h"
#include "evtimer_msg.h"
#include "timer_units.h"
#include "time.h"
#include "timex.h"
#include "netinet/in.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "mbox.h"
#include "unistd.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("Initializing RTC\n");

    struct tm timeinfo = {
        .tm_year = 2024 - 1900,  // Years since 1900
        .tm_mon = 6 - 1,         // Months since January
        .tm_mday = 1,            // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0,
        .tm_isdst = -1,          // Not considering daylight saving
    };

    time_t epoch = mktime(&timeinfo);
    rtc_time_set(epoch);

    // Set an alarm for 5 seconds from the current time
    rtc_alarm_t alarm = {
        .time = epoch + 5,
        .callback = alarm_callback,
        .arg = NULL,
    };
    rtc_set_alarm(&alarm);

    while (1) {
        xtimer_sleep(60);
    }

    return 0;
}