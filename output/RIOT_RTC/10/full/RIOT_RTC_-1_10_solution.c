#include "string.h"
#include "net/sock/util.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "paho_mqtt.h"
#include "stdbool.h"
#include "ztimer.h"
#include "msg.h"
#include "esp_system.h"
#include "timex.h"
#include "stddef.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "time_units.h"
#include "sys/socket.h"
#include "periph/i2c.h"
#include "net/sock/udp.h"
#include "mutex.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "xtimer.h"
#include "thread.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"

static void my_alarm_callback(void){
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_init();

    struct tm time;
    time.tm_year = 2024 - 1900;  // Years since 1900
    time.tm_mon = 5;             // 0-based, so June is 5
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    time_tm_set(&time);

    // Set alarm to go off 5 seconds from now
    xtimer_sleep(5);

    my_alarm_callback();

    while (1) {
        // The main execution loop handles other tasks
    }

    return 0;
}