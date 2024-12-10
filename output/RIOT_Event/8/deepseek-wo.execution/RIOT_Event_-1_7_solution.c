
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <thread.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <stdio.h>
#include <mutex.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <dht.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>
#include <net/gcoap.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

void event_periodic_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Event periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;
    msg_t msg;

    // Initialize the message queue for the current thread
    msg_t msg_array[8];
    msg_init_queue(msg_array, 8);

    // Set the timer to trigger the callback every second
    xtimer_set(&timer, TIMER_INTERVAL, event_periodic_callback, NULL);

    // Main loop to keep the thread alive
    while (1) {
        msg_receive(&msg);
    }

    return 0;
}
