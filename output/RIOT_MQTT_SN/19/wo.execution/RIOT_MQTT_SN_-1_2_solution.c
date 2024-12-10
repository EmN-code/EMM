#include <string.h>
#include <stdio.h>
#include "netinet/in.h"
#include "net/gcoap.h"
#include "net/emcute.h"

#define EMCUTE_PORT 1883
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_TOPIC "state"
#define INTERVAL 5

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    char *msg = (char *)data;
    printf("Received publication on topic "%s": %.*s\n", topic->name, (int)len, msg);
}

static void publish_state(void) {
    const char *message = "work";
    size_t len = strlen(message);

    // Publish the "work" state
    if (emcute_pub(&topic, message, len, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Failed to publish work state");
    }
}

int main(void) {
    memset(stack, 0, sizeof(stack));
    memset(queue, 0, sizeof(queue));

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: Unable to parse broker IP address");
        return 1;
    }

    // Thread initialization
    emcute_run(EMCUTE_PORT, "esp32");

    // Connect to the broker's gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Failed to connect to MQTT-SN broker");
        return 1;
    } else {
        puts("Successfully connected to MQTT-SN broker");
    }

    // Prepare the MQTT topic
    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Subscription failed");
        return 1;
    }

    while (1) {
        publish_state();
        // Sleep for the defined interval
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
