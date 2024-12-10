#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <msg.h>
#include <assert.h>
#include <evtimer_mbox.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <stdatomic.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <thread.h>
#include <mbox.h>

#define INTERVAL (1U * US_PER_SEC)

static void evtimer_callback(void *arg) {
    (void)arg;
    printf("Event Timer Callback triggered.\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_mbox_event_t event;
    kernel_pid_t my_pid = thread_getpid();
    mbox_t my_mbox;
    msg_t my_queue[16];

    mbox_init(&my_mbox, my_queue, 16);
    evtimer_init(&evtimer);

    evtimer_periodic_init(&event.event, INTERVAL);
    evtimer_periodic_set_callback(&event.event, evtimer_callback, NULL);
    evtimer_add_mbox(&evtimer, &event, &my_mbox);

    while (1) {
        msg_t msg;
        mbox_get(&my_mbox, &msg);
        evtimer_periodic_add(&evtimer, &event);
        evtimer_process();
    }

    return 0;
}