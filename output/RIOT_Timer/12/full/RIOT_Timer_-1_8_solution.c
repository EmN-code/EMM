#include "net/sock/util.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "netinet/in.h"
#include "evtimer_mbox.h"
#include "paho_mqtt.h"
#include "stdbool.h"
#include "ztimer.h"
#include "msg.h"
#include "esp_system.h"
#include "dht_params.h"
#include "timex.h"
#include "dht.h"
#include "stddef.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "sys/socket.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "periph/i2c.h"
#include "periph_conf.h"
#include "architecture.h"
#include "mutex.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "xtimer.h"
#include "mma8x5x_params.h"
#include "thread.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"

// Fixes the errors by correcting the use of event timer struct in RIOT

static evtimer_t evtimer;
static evtimer_event_t event;

void cb(void *arg) {
    (void)arg;
    // Simulate turning on an LED
    printf("LED ON\n");
}

int main(void) {
    evtimer_event_add(&evtimer, &event, 1, cb, NULL);

    while (1) {
        evtimer_run(&evtimer);
        // Sleep for 1 second, replace problematic usleep
        sleep(1);
    }

    return 0;
}