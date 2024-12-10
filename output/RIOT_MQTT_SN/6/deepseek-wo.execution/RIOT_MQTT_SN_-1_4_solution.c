
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <stdio.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = EMQX_SERVER_PORT;

    // Convert IP address string to byte representation
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    int connect_result = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (connect_result != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN gateway");
        return 1;
    }

    // Register the topic
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    int reg_result = emcute_reg(&topic);
    if (reg_result != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    // Publish the message every 5 seconds
    while (1) {
        int pub_result = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0);
        if (pub_result != EMCUTE_OK) {
            puts("Error publishing message");
        }
        xtimer_sleep(5);
    }

    return 0;
}
