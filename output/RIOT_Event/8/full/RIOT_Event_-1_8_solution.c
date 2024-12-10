#include <ztimer.h>  // Correct header for timer operations
#include <stdbool.h>
#include <stdatomic.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <event/thread.h> 
#include <mbox.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <stdio.h>
#include <stdlib.h>
#include <dht_params.h>
#include <mma8x5x_params.h>
#include <net/emcute.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <event/callback.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <mma8x5x.h>
#include <net/sock/udp.h>
#include <thread.h>
#include <arpa/inet.h>
#include <timex.h>
#include <esp_system.h>
#include <sys/socket.h>
#include <net/gcoap.h>

static void periodic_event_handler(void *arg)
{
    // Example implementation of the periodic event handler
    printf("Periodic event triggered.\n");
}

int main(void)
{
    // Initialize evtimer
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    msg_t m;
    event.event.interval = 1 * 1000000U; // 1 second
    
    evtimer_init_msg(&evtimer);
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    while (true) {
        msg_receive(&m);
        periodic_event_handler(NULL);
    }
    return 0;
}