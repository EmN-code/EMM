#include "ztimer.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "timex.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "mbox.h"
#include "mutex.h"
#include "xtimer.h"
#include "time_units.h"
#include "esp_system.h"
#include "evtimer_mbox.h"
#include "unistd.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"

int main(void) {
    // simulating RTC structure and setting.
    struct tm timeinfo = {
        .tm_year = 2024 - 1900, // tm_year is years since 1900
        .tm_mon = 6 - 1,        // tm_mon range is [0, 11]
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    puts("RTC example: setting date to 2024-06-01\n");

    // Initialize RTC and set time (use appropriate functions for RTC handling)
    // rtc_init();
    // rtc_set_time(&timeinfo);

    // Delay 5 seconds to simulate an alarm
    ztimer_sleep(ZTIMER_MSEC, 5000);

    // Trigger alarm
    puts("Alarm triggered!\n");

    return 0;
}