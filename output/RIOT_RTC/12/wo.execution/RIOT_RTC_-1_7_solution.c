#include "xtimer.h"
#include "ztimer.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "unistd.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "timex.h"
#include "time_units.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_system.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "kernel_defines.h"

void *alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
    return NULL;
}

int main(void) {
    struct tm date;
    date.tm_year = 2024 - 1900; // Year since 1900
    date.tm_mon = 6 - 1;  // Month, where 0 = jan
    date.tm_mday = 1;    // Day of the month
    date.tm_hour = 0;
    date.tm_min = 0;
    date.tm_sec = 0;

    rtc_init();
    rtc_set_time(&date);

    struct timeval alarm_time;
    gettimeofday(&alarm_time, NULL); // Get current time
    alarm_time.tv_sec += 5;  // Set the alarm 5 seconds into the future

    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    while (1) {
        ; // Keep running
    }
    return 0;
}