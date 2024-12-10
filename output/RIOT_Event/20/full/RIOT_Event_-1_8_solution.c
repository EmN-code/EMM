#include "esp_system.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "xtimer.h"
#include "ztimer.h"
#include "arpa/inet.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mbox.h"
#include "msg.h"
#include "mutex.h"

evtimer_t periodic_timer;
msg_t queue[8];

void event_periodic_callback(void *arg)
{
    (void)arg; // Suppress unused parameter warning
    printf("Periodic event triggered!\n");
}

int main(void)
{
    msg_init_queue(queue, 8);
    evtimer_init(&periodic_timer, event_periodic_callback, NULL);
    evtimer_set_period(&periodic_timer, 1000000); // 1 second in microseconds
    evtimer_add(&periodic_timer);

    while (1) {
        evtimer_run();
        msg_receive(&queue[0]);
    }
    return 0;
}