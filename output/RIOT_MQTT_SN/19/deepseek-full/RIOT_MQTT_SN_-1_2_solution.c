
#include <net/emcute.h>
#include <net/sock/util.h>
#include <net/ipv6/addr.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <sys/socket.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"
#define EMCUTE_BUFSIZE 128

static emcute_topic_t topic;
static sock_udp_ep_t gw;

static void _publish_state(void)
{
    char buf[EMCUTE_BUFSIZE];
    strncpy(buf, STATE_MESSAGE, sizeof(buf));
    emcute_pub(&topic, buf, strlen(buf), 0);
}

int main(void)
{
    // Initialize the gateway endpoint
    gw.family = AF_INET;
    gw.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_ADDR, &gw.addr.ipv4);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: Could not connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: Could not register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        _publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
