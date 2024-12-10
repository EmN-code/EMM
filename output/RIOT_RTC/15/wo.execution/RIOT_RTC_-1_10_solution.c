#include "ztimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "sys/socket.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "ztimer.h"
#include "msg.h"
#include "mutex.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph_cpu.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "event/thread.h"
#include "event/periodic.h"
#include "event/callback.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"

static void rtc_alarm_cb(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1;  // Month, range 0-11
    time.tm_mday = 1;    // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_init();
    rtc_set_time(mktime(&time));

    // Setup the alarm to trigger in 5 seconds
    ztimer_sleep(ZTIMER_MSEC, 5000);
    rtc_set_alarm(mktime(&time) + 5, rtc_alarm_cb, NULL);

    // Main loop does nothing, just waits for the alarm
    while (1) {
        sleep(1);
    }
    return 0;
}