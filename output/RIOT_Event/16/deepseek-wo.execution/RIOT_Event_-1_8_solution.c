
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <event/thread.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <esp_system.h>
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
#include <net/gcoap.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t periodic_timer;

void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
    // Re-add the event to keep the periodic behavior
    evtimer_add(&periodic_timer, (evtimer_event_t *)arg, PERIODIC_INTERVAL);
}

int main(void) {
    evtimer_event_t event;

    // Initialize the event timer
    evtimer_init(&periodic_timer, periodic_callback);

    // Initialize the event
    event.next = NULL;
    event.offset = 0;

    // Add the event to the timer with the specified interval
    evtimer_add(&periodic_timer, &event, PERIODIC_INTERVAL);

    // Main loop to keep the thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
