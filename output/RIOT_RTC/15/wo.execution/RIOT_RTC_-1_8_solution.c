#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/socket.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph_cpu.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "event/callback.h"
#include "event/periodic.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"
#include "arpa/inet.h"
#include "paho_mqtt.h"
#include "msg.h"

void alarm_callback(void *arg) {
  printf("Alarm triggered!\n");
}

int main(void) {
    printf("RTC Alarm Example\n");
    rtc_init();  // Initialize the RTC

    // Set the RTC time to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900;
    time.tm_mon = 6 - 1;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    time.tm_isdst = -1;

    time_t t = mktime(&time);
    rtc_set_time(t);  // This line had error earlier
    printf("Time set to: %s", asctime(&time));

    // Set an alarm to trigger after 5 seconds
    rtc_alarm_t alarm;
    alarm.callback = alarm_callback;
    alarm.arg = NULL;
    rtc_set_alarm(t + 5);  // This line had error earlier

    while (1) {
        sleep(1);  // Sleep the main thread
    }

    return 0;
}
