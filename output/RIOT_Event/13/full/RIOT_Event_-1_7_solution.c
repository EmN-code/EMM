#include "ztimer.h"
#include <xtimer.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <esp_system.h>
#include <msg.h>
#include <unistd.h>
#include <timex.h>
#include "net/sock/util.h"
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time_units.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <thread.h>
#include <event/thread.h>
#include <event/periodic.h>
#include <event/callback.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <kernel_defines.h>

void callback(void *arg) {
    (void)arg; // suppress unused parameter warning
    printf("Event triggered!\n");
}

int main(void) {
    /* Use ztimer to call a function every second */
    ztimer_t timer;
    ztimer_set(ZTIMER_MSEC, &timer, 1000, callback, NULL);

    /* Use an event loop to keep the program running */
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}