#include "evtimer_mbox.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "kernel_defines.h"
#include "net/gcoap.h"
#include "timex.h"
#include "msg.h"
#include "xtimer.h"
#include "mbox.h"
#include "arpa/inet.h"
#include "ztimer.h"
#include "stdatomic.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "stdbool.h"
#include "stddef.h"
#include "time_units.h"
#include "net/emcute.h"
#include "unistd.h"
#include "net/sock/udp.h"
#include "stdlib.h"
#include "evtimer_msg.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Set RTC time to June 1, 2024
    struct tm time_info = {
        .tm_year = 2024 - 1900,  // Years since 1900
        .tm_mon = 6 - 1,         // Months since January
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    rtc_set_time(&time_info);

    // Set an alarm to trigger after 5 seconds
    ztimer_sleep(ZTIMER_MSEC, 5000);
    alarm_callback(NULL);

    return 0;
}