
#include <stdlib.h>
#include <net/emcute.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <event/thread.h>
#include <time_units.h>
#include <xtimer.h>
#include <esp_system.h>
#include <thread.h>
#include <mma8x5x_params.h>
#include <mutex.h>
#include <dht_params.h>
#include <stdio.h>
#include <MQTTClient.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <dht.h>
#include <esp_wifi.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <evtimer.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
    xtimer_set(&timer, PERIODIC_INTERVAL);
}

static xtimer_t timer;

int main(void)
{
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    timer.callback = periodic_callback;
    timer.arg = NULL;

    xtimer_set(&timer, PERIODIC_INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle messages if needed
    }

    return 0;
}
