#include <xtimer.h>
#include <ztimer.h>
#include <sys/socket.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <event/callback.h>
#include <esp_system.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <thread.h>
#include <unistd.h>
#include <timex.h>

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval

static void _event_periodic_callback(void* arg)
{
    (void)arg;  // Avoid unused parameter warning
    printf("Periodic event triggered!\n");
}

int main(void)
{
    ztimer_t timer;
    ztimer_set(&timer, ZTIMER_MSEC, INTERVAL, _event_periodic_callback, NULL);

    // Run the event loop
    while (1) {
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }

    return 0;
}