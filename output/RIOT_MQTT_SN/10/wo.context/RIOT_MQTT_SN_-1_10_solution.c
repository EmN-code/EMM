#include <xtimer.h>
#include <emcute.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <stdlib.h>

#define EMCUTE_PORT     (1883U)
#define SERVER_ADDRESS  "47.102.103.1"
#define STATE_TOPIC     "device/state"
#define PUBLISH_INTERVAL (5U)  // 5 seconds
#define HOST_ID         "esp32_node"

static emcute_topic_t topic;
static char pub_data[] = "work";  // Ensure data is short enough to not overflow

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    // Empty function to prevent unused-function error.
}

static void mqtt_pub(void) {
    emcute_pub(&topic, pub_data, strlen(pub_data), EMCUTE_QOS_0);
}

int main(void) {
    // Initialize the network interface
    sock_udp_ep_t gw = {.family = AF_INET, .port = EMCUTE_PORT};
    inet_pton(AF_INET, SERVER_ADDRESS, &gw.addr.ipv4);
    
    // Initialize emcute
    emcute_run(EMCUTE_PORT, HOST_ID);

    // Connect to MQTT-SN broker
    emcute_con(&gw, true, NULL, NULL, 0, 0);

    // Register topic
    topic.name = STATE_TOPIC;
    emcute_reg(&topic);

    while (1) {
        mqtt_pub();
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL * 1000); // Using ztimer_sleep instead of xtimer_sleep
    }

    return 0;
}