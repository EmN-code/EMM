
#include <net/ipv6/addr.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mutex.h>
#include <stdio.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work"
#define MESSAGE_LEN 4

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    emcute_topic_t topic;
    char topic_name[CONFIG_EMCUTE_TOPIC_MAXLEN];

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN gateway");
        return 1;
    }

    // Register the topic
    strncpy(topic_name, TOPIC_NAME, sizeof(topic_name) - 1);
    topic_name[sizeof(topic_name) - 1] = '\0';
    topic.name = topic_name;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return 1;
    }

    // Publish state every five seconds
    while (1) {
        if (emcute_pub(&topic, MESSAGE, MESSAGE_LEN, 0) != EMCUTE_OK) {
            puts("Error: could not publish message");
        } else {
            puts("Message published successfully");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
