
#include <stdbool.h>
#include <stdatomic.h>
#include <ztimer.h>
#include <net/emcute.h>
#include <arpa/inet.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work"

static sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
static emcute_topic_t topic;

int main(void)
{
    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid IP address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN gateway");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return 1;
    }

    // Publish state every five seconds
    while (1) {
        if (emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0) != EMCUTE_OK) {
            puts("Error: could not publish message");
        } else {
            puts("Message published");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}