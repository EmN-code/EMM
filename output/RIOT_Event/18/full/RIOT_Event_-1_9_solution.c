#include <stdlib.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <msg.h>
#include <xtimer.h>
#include <event/thread.h>
#include <kernel_defines.h>

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];

static void event_periodic(void *arg) {
    (void)arg;
    puts("Periodic event triggered");
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    xtimer_ticks32_t last_wakeup = xtimer_now();
    
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        event_periodic(NULL);  // Trigger the periodic callback
    }
    return 0;
}