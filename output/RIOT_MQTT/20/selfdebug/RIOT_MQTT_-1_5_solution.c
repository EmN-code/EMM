#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>

#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // 5 seconds

static void publish_state(void) {
    // Simulate state: just toggling between these two states
    static int work_state = 0;
    const char *state = work_state ? "work" : "not work";
    work_state = 1 - work_state;

    printf("Publishing state: %s\n", state);
    // Here should be the code to send the state to the broker
    // using some non-emcute MQTT library functions
}

int main(void) {
    // Initialize network, MQTT client, etc.
    printf("Starting MQTT client to publish state every 5 seconds\n");

    // Timer to schedule the state publish every 5 seconds
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        publish_state();
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
    }

    return 0; // This won't actually be reached
}