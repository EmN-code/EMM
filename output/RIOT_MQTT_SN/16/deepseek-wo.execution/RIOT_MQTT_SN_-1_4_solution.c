
#include <net/ipv6/addr.h>
#include <mma8x5x_params.h>
#include <net/emcute.h>
#include <mutex.h>
#include <netinet/in.h>
#include <mma8x5x_regs.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <mma8x5x.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>

#define TOPIC_NAME "state"
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MSG_SIZE 15
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_t sock;
static sock_udp_ep_t broker = SOCK_IPV4_EP_ANY;

static void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static unsigned state_index = 0;

    char msg[MSG_SIZE];
    snprintf(msg, sizeof(msg), "%s", states[state_index]);
    state_index = (state_index + 1) % ARRAY_SIZE(states);

    if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    }
}

int main(void) {
    // Initialize the MQTT-SN client
    if (emcute_init() != EMCUTE_OK) {
        puts("Error: unable to initialize MQTT-SN client");
        return 1;
    }

    // Set up the broker endpoint
    broker.family = AF_INET;
    broker.port = HTONS(BROKER_PORT);
    if (inet_aton(BROKER_IP, &broker.addr.ipv4) == 0) {
        puts("Error: invalid broker IP address");
        return 1;
    }

    // Connect to the broker
    if (emcute_con(&broker, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
