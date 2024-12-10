
#include <net/emcute.h>
#include <net/sock/util.h>
#include <net/ipv6/addr.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <mma8x5x_regs.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;

static void _publish_state(void)
{
    static bool state = true;
    const char *msg = state ? MSG_WORK : MSG_NOT_WORK;
    state = !state;

    if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    }
}

int main(void)
{
    // Initialize the gateway endpoint
    gw.port = BROKER_PORT;
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, BROKER_ADDR) == NULL) {
        puts("Error: unable to parse broker address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
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
        _publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
