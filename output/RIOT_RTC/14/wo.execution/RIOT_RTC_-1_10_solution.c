#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

void alarm_callback(struct tm *now, void *arg) {
    (void)now;
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_init();

    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1; // Month, January is 0
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    time.tm_isdst = -1;

    rtc_set_time(&time);

    struct tm alarm_time = time;
    xtimer_sleep(5); // Simulating the delay using xtimer, since real RTC alarms may not function properly in a simulated environment

    rtc_get_time(&alarm_time);

    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    return 0;
}