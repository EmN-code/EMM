#include <string.h>
#include <net/sock/udp.h>
#include <net/mqttsn.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32-client"
#define PUB_TOPIC_NAME "state"
#define PUB_INTERVAL_S 5

static emcute_topic_t topic;

void publish_state(void) {
    const char *state = "work"; // Change to "not work" as needed.
    int res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Publish failed with error code %d\n", res);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

    // Convert IP address
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) != 1) {
        printf("Error: unable to parse broker IP address\n");
        return 1;
    }

    // Configure and connect to the broker
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Connection to gateway failed with error code %d\n", res);
        return 1;
    }

    // Set up the topic we wish to publish to
    topic.name = PUB_TOPIC_NAME;
    emcute_reg(&topic);

    // Publish periodically
    while (1) {
        publish_state();
        xtimer_sleep(PUB_INTERVAL_S);
    }

    return 0;
}