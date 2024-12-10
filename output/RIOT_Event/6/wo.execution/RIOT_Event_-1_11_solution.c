#include <xtimer.h>
#include <ztimer.h>
#include <stdio.h>
#include <evtimer_mbox.h>
#include <stdatomic.h>
#include <time_units.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <timex.h>
#include <msg.h>
#include <mbox.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <esp_wifi.h>
#include <dht_params.h>
#include <sys/socket.h>
#include <mma8x5x.h>
#include <esp_system.h>
#include <mma8x5x_regs.h>
#include <stdbool.h>
#include <mma8x5x_params.h>
#include <dht.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <stdlib.h>
#include <evtimer_msg.h>
#include <unistd.h>

#define ONE_SECOND 1000000U

static evtimer_t evtimer;
static evtimer_msg_event_t event;

void callback_function(void *arg) {
    (void)arg;
    printf("Callback triggered!\n");
}

int main(void) {
    /* Initialize the event timer */
    evtimer_init_msg(&evtimer);

    /* Set up the event callback */
    event.event.callback = callback_function;
    event.event.period = ONE_SECOND;

    /* Adding the event to the timer */
    evtimer_add_msg(&evtimer, &event, kernel_pid_t)

    while (1) {
        /* We sleep to let the event handler do its job. */
        ztimer_sleep(ZTIMER_SECOND);
        msg_t m;
        msg_receive(&m);
        printf("Message received.\n");
    }
    return 0;
}