#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mbox.h"
#include "msg.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "time_units.h"
#include "stdatomic.h"

// Function used to simulate alarm
void alarm_handler(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    // Assume we will set June 1, 2024 via RTC-like method
    struct tm time;
    time.tm_year = 2024 - 1900;
    time.tm_mon = 5; // June is 5 as it is 0-indexed
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Here we assume rtc set equivalent would be implemented
    // rtc_set_time(&time); // This should be replaced with actual implementation

    // Create a timer to trigger alarm in 5 seconds
    ztimer_sleep(ZTIMER_MSEC, 5000);
    alarm_handler(NULL);

    return 0;
}