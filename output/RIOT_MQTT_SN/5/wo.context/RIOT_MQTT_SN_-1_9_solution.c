#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <emcute.h>

#define MQTT_BROKER "47.102.103.1"
#define DEFAULT_PORT 1883
#define PUB_INTERVAL_SEC 5

static emcute_topic_t topic;
static char *topics[] = {"work/state"};

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    // Implement the handling of received data here.
    (void)topic;
    (void)data;
    (void)len;
}

int main(void) {
    static sock_udp_ep_t gw = { .family = AF_INET6, .port = DEFAULT_PORT };
    
    // Initialize the gateway address
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER) == NULL) {
        puts("ERROR: unable to parse broker address\n");
        return 1;
    }

    puts("MQTT-SN publisher application for RIOT
");

    // Setup network stack and connect to gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Cannot connect to broker.");
        return 1;
    }

    // Set up topic
    topic.name = topics[0];

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Failed to register topic.");
        return 1;
    }

    // Subscribe to the topic
    static emcute_sub_t sub = { next: NULL, topic: topic, cb: on_pub, arg: NULL };
    if (emcute_sub(&sub, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Failed to subscribe to topic.");
        return 1;
    }

    while (1) {
        // Publish state every PUB_INTERVAL_SEC
        char *work_state = "work";
        if (emcute_pub(&topic, work_state, strlen(work_state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Failed to publish data.");
        }
        sleep(PUB_INTERVAL_SEC);
    }

    return 0;
}