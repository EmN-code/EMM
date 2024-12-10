#include "net/sock/util.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "stdbool.h"
#include "ztimer.h"
#include "msg.h"
#include "dht_params.h"
#include "esp_system.h"
#include "timex.h"
#include "stddef.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "sys/socket.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "architecture.h"
#include "mutex.h"
#include "net/emcute.h"
#include "event/callback.h"
#include "stdlib.h"
#include "xtimer.h"
#include "mma8x5x_params.h"
#include "thread.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"

void callback_function(void) {
    printf("Callback function triggered every second.\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_event_t evtimer_event;

    evtimer_init(&evtimer, _evtimer_msg_handler);
    evtimer_event.callback = callback_function;

    evtimer_add(&evtimer, &evtimer_event);

    while (1) {
        for (int i = 0; i < 10; i++) {
            evtimer_add(&evtimer, &evtimer_event);
            usleep(1000000);
        }
    }
    return 0;
}