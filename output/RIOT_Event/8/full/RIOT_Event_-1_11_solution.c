#include <xtimer.h>
#include <ztimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <dht.h>
#include <dht_params.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <sys/socket.h>
#include <timex.h>
#include <unistd.h>
#include <event/thread.h>
#include <event/callback.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <stdatomic.h>

#define PERIODIC_INTERVAL (1U * US_PER_SEC)  // 1 second interval

void _periodic_task(void) {
    printf("Periodic task executed\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    msg_init_queue((msg_t[8]){}, 8);
    evtimer_init_msg(&evtimer);

    event.msg.type = 0; // Type to identify the event
    event.msg.content.ptr = &_periodic_task;

    while (1) {
        evtimer_add_msg(&evtimer, &event, KERNEL_PID_MAIN);
        xtimer_sleep(1);
    }

    return 0;
}