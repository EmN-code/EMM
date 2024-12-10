
#include <stdio.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <net/sock/udp.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <event/thread.h>
#include <esp_wifi.h>
#include <net/gcoap.h>
#include <MQTTClient.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <thread.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <stddef.h>
#include <sys/socket.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;
static sock_udp_ep_t gw_ep = { .family = AF_INET, .port = EMQX_SERVER_PORT };

static void publish_state(void)
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
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 12345; /* Arbitrary local port */

    /* Convert IP address string to binary form */
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &gw_ep.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    /* Create UDP socket */
    if (sock_udp_create(&sock, &local, &gw_ep, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    /* Connect to MQTT-SN gateway */
    if (emcute_con(&gw_ep, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN gateway");
        return 1;
    }

    /* Register topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
    }

    return 0;
}
