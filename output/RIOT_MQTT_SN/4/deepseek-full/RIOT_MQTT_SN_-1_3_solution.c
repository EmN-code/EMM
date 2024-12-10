
#include <net/sock/udp.h>
#include <mma8x5x_regs.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <mma8x5x.h>
#include <net/emcute.h>
#include <arpa/inet.h>
#include <dht_params.h>
#include <kernel_defines.h>
#include <net/ipv6/addr.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_CONTENT "work or not work"
#define MSG_LEN (sizeof(MSG_CONTENT) - 1)
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;
static sock_udp_ep_t gw_ep = { .family = AF_INET, .port = EMQX_SERVER_PORT };

static void _publish_state(void)
{
    unsigned flags = 0;
    int res = emcute_pub(&topic, MSG_CONTENT, MSG_LEN, flags);
    if (res != EMCUTE_OK) {
        puts("Error publishing message");
    }
}

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = { .family = AF_INET };

    /* Convert IP address string to byte representation */
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &gw_ep.addr.ipv4) <= 0) {
        puts("Error parsing IP address");
        return 1;
    }

    /* Create UDP socket */
    if (sock_udp_create(&sock, &local, &gw_ep, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    /* Connect to MQTT-SN broker */
    if (emcute_con(&gw_ep, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    /* Register topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        _publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
