#include <ztimer.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include <time_units.h>
#include <timex.h>
#include <unistd.h>
#include <architecture.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <stdio.h>
#include <mbox.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <thread.h>
#include <mqttclient.h>
#include <paho_mqtt.h>
#include <mutex.h>

#include "network_adapter.h"  // Hypothetical network adapter functions

static ztimer_t timer;

void timer_callback(void *arg) {
    // Hypothetical LED toggle function
    toggle_led_mock();
}

static void setup_event_timer(void) {
    /* Initialize the timer, set it to trigger every second, with the timer_callback function */
    ztimer_set(&timer, ZTIMER_MSEC, 1000, timer_callback, NULL);
}

int main(void) {
    Network network;

    while (1) {
        int rc = NetworkConnect(&network, "192.168.1.100", 1883);
        if (rc != 0) {
            printf("Network connection failed. Retrying...\n");
            usleep(1000000); // Retry after 1 second
            continue;
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }

    setup_event_timer();

    while (1) {
        sleep(1);  // Main loop does nothing, waits for timer events
    }
    return 0;
}
