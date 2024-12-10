#include <net/sock/util.h>
#include <stdatomic.h>
#include <MQTTClient.h>
#include <net/gcoap.h>
#include <evtimer_mbox.h>
#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <ztimer.h>
#include <msg.h>
#include <dht_params.h>
#include <timex.h>
#include <stddef.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <sys/socket.h>
#include <time_units.h>
#include <net/sock/udp.h>
#include <architecture.h>
#include <net/emcute.h>
#include <event/callback.h>
#include <stdlib.h>
#include <xtimer.h>
#include <mma8x5x_params.h>
#include <thread.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <event/thread.h>

void periodic_callback(struct evtimer_event *event) {
    printf("Periodic event triggered\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_init_mbox(&evtimer);

    evtimer_mbox_event_t event;
    event.callback = periodic_callback;
    event.period = 1000000U; // 1 second in microseconds

    mbox_t mbox;
    mbox_init(&mbox); 

    evtimer_add_mbox(&evtimer, &event, &mbox);

    while (1) {
        evtimer_run(&evtimer);
        xtimer_usleep(1000000);
    }

    return 0;
}