#include <net/emcute.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL 5U

static emcute_topic_t topic;

static void publish_state(void) {
    const char *state = "work";
    emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
}

int main(void) {
    // Setup connection endpoint
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = BROKER_PORT
    };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    // Initialize MQTT-SN connection structures
    char *will_topic = NULL;
    void *will_msg = NULL;

    // Connecting to the MQTT-SN broker
    if (emcute_con(&gw, true, will_topic, will_msg, 0, EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Connection failed.\n");
        return 1;
    }

    // Register topic
    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Registration of topic failed.\n");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}