#include "xtimer.h"
#include "mutex.h"
#include "msg.h"
#include "event/thread.h"
#include "stdatomic.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "esp_system.h"
#include "periph/rtc.h"
#include "timex.h"
#include "time.h"
#include "ztimer.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "arpa/inet.h"

#define INTERVAL (1U * XTIMER_SEC)  // 1 second interval

static void timer_callback(void *arg) {
    (void)arg;  // Unused
    puts("Callback executed");
}

int main(void) {
    puts("Starting periodic timer example");

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        timer_callback(NULL);
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return 0;
}