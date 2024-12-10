#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "msg.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "arpa/inet.h"
#include "time_units.h"
#include "timex.h"
#include "mbox.h"

void periodic_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Periodic event triggered every second!\n");
}

int main(void) {
    evtimer_msg_event_t event;
    ztimer_clock_t *clock = ZTIMER_USEC;
    msg_t queue[8];

    printf("Starting periodic event example\n");

    // Initialize message queue
    msg_init_queue(queue, 8);

    // Initialize the event timer
    evtimer_init_msg(&event, queue);

    // Set the event timer to call the callback function every second
    evtimer_add_msg(ZTIMER_MSEC, 1000, &event, periodic_callback, NULL);

    // Wait forever for periodic events
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}