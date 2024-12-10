
#include <net/sock.h>
#include <net/emcute.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work"

static emcute_topic_t topic;

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = EMQX_SERVER_PORT;

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: Invalid IP address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: Could not connect to MQTT-SN gateway");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: Could not register topic");
        return 1;
    }

    // Publish state every five seconds
    while (1) {
        if (emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0) != EMCUTE_OK) {
            puts("Error: Could not publish message");
        } else {
            puts("Message published successfully");
        }
        xtimer_sleep(5);
    }

    return 0;
}
