
#include <thread.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define TOPIC_NAME "state"
#define STATE_MSG "work"
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = SOCK_IPV6_EP_ANY;

void publish_state(void) {
    while (1) {
        int res = emcute_pub(&topic, STATE_MSG, sizeof(STATE_MSG) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish state: %d\n", res);
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }
}

int main(void) {
    // Initialize the MQTT-SN topic
    topic.name = TOPIC_NAME;
    topic.id = 0;

    // Set up the gateway endpoint
    gw.family = AF_INET;
    gw.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN gateway: %d\n", res);
        return 1;
    }

    // Register the topic
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    // Start the publishing thread
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  publish_state, NULL, "publish_state");

    return 0;
}
