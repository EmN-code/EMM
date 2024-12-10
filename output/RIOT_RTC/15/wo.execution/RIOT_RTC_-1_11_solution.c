#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "event/callback.h"
#include "event/periodic.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "periph_cpu.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "kernel_defines.h"

static void alarm_handler(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;

    /* Set the RTC time to June 1, 2024, 00:00:00 */
    timeinfo.tm_year = 2024 - 1900;  // Year since 1900
    timeinfo.tm_mon = 5;             // Months since January [0-11]
    timeinfo.tm_mday = 1;            // Day of the month [1-31]
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    /* Set RTC time */
    rtc_set_time(&timeinfo);

    /* Set an RTC alarm for 5 seconds from now */
    struct tm alarm_time = timeinfo;
    alarm_time.tm_sec += 5;
    rtc_set_alarm(&alarm_time, alarm_handler, NULL);

    while (1) {
        /* main loop does nothing, just waiting for the alarm */
        xtimer_sleep(10);
    }

    return 0;
}