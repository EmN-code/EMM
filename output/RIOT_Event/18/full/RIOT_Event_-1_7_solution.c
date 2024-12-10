#include "mma8x5x_params.h"
#include "dht_params.h"
#include "net/emcute.h"
#include "stddef.h"
#include "event/thread.h"
#include "net/sock/udp.h"
#include "stdatomic.h"
#include "paho_mqtt.h"
#include "stdbool.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"
#include "netinet/in.h"
#include "dht.h"
#include "time_units.h"
#include "evtimer.h"
#include "mutex.h"
#include "event/callback.h"
#include "unistd.h"
#include "stdlib.h"
#include "arpa/inet.h"
#include "thread.h"
#include "esp_system.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "sys/socket.h"
#include "esp_wifi.h"
#include "net/gcoap.h"
#include "xtimer.h"
#include "msg.h"
#include "stdio.h"
#include "ztimer.h"
#include "timex.h"
#include "mma8x5x_regs.h"
#include "event/timeout.h"
#include "net/sock/util.h"
#include "evtimer_msg.h"

void my_periodic_event(void *arg) {
    (void)arg;
    printf("Event triggered\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_msg_event_t event;

    evtimer_init_msg(&evtimer);

    uint32_t interval = 1000000; // 1 second in microseconds

    evtimer_msg_event_init(&event, &evtimer, thread_getpid());
    evtimer_msg_add(&evtimer, &event, interval);

    while (1) {
        evtimer_msg_wait();
        my_periodic_event(NULL);
    }

    return 0;
}