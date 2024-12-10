#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <timer.h>
#include <sys/socket.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <esp_wifi.h>
#include <esp_system.h>

void event_handler(void) {
    printf("Event triggered!\n");
}

int main(void) {
    printf("Event timer example on ESP32\n");

    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    evtimer_t evtimer;
    evtimer_msg_event_t event;

    evtimer_msg_event_init(&event);
    evtimer_msg_event_set(&event, ZTIMER_SEC);

    evtimer_init_msg(&evtimer);
    evtimer_add_msg(&evtimer, &event, kernel_pid_t());

    while (1) {
        msg_receive(&msg_queue[0]);
        event_handler();
    }

    return 0;
}