
#include <stdlib.h>
#include <stdbool.h>
#include <net/emcute.h>
#include <ztimer.h>
#include <net/ipv6/addr.h>
#include <esp_system.h>
#include <time_units.h>
#include <stddef.h>
#include <unistd.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * US_PER_SEC)

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };

void publish_state(void) {
    const char *state = "work";
    emcute_pub(&topic, state, strlen(state), 0);
}

int main(void) {
    // Initialize the ESP32 system
    esp_system_init();

    // Convert the IP address string to a byte-represented IP address
    inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: Could not connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: Could not register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
    }

    return 0;
}
