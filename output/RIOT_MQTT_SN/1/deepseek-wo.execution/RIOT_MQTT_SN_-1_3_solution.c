
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <net/gcoap.h>
#include <mutex.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"

static sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
static emcute_topic_t topic;

int main(void)
{
    // Initialize the UDP socket
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error converting IP address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN gateway");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    // Publish state every five seconds
    while (1) {
        if (emcute_pub(&topic, STATE_MESSAGE, sizeof(STATE_MESSAGE) - 1, 0) != EMCUTE_OK) {
            puts("Error publishing message");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
