
#include <thread.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <kernel_defines.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <arpa/inet.h>
#include <dht_params.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"

static emcute_sub_t sub;
static emcute_topic_t topic;

void mqtt_callback(void *arg, const emcute_topic_t *topic, void *data, size_t len)
{
    (void)arg;
    (void)topic;
    (void)data;
    (void)len;
    // Handle incoming messages if needed
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    sock_udp_t sock;

    local.port = 1883;
    remote.family = AF_INET;
    remote.port = EMQX_SERVER_PORT;

    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid address");
        return 1;
    }

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: could not create UDP socket");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return 1;
    }

    sub.topic = topic;
    sub.cb = (emcute_cb_t)mqtt_callback;
    if (emcute_sub(&sub, 0) != EMCUTE_OK) {
        puts("Error: could not subscribe to topic");
        return 1;
    }

    while (1) {
        const char *msg = (rand() % 2 == 0) ? MSG_WORK : MSG_NOT_WORK;
        if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
            puts("Error: could not publish message");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
