#include "dht.h"
#include "dht_params.h"
#include "event/callback.h"
#include "event/thread.h"
#include "evtimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"
#include "mutex.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "paho_mqtt.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "thread.h"
#include "time_units.h"
#include "timex.h"
#include "unistd.h"
#include "xtimer.h"
#include "ztimer.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"

void event_periodic_handler(void *arg)
{
    printf("Event triggered every second\n");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    msg_t msg_queue[8];

    msg_init_queue(msg_queue, 8);

    evtimer_init_msg(&evtimer);

    evtimer_msg_event_init(&event, &event_periodic_handler, NULL);
    evtimer_add_msg(&evtimer, &event.event, thread_getpid());

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == EVENT_MSG_TYPE && msg.content.ptr == &event) {
            event_periodic_handler(NULL);
        }
    }

    return 0;
}