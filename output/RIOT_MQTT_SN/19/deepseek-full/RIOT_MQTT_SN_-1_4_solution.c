
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <arpa/inet.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <net/sock/udp.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <event/thread.h>
#include <net/gcoap.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <sys/socket.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work"

static emcute_topic_t topic;
static sock_udp_ep_t gw_ep = SOCK_IPV4_EP_ANY;

void publish_state(void) {
    const char *message = STATE_MESSAGE;
    emcute_pub(&topic, message, strlen(message), 0);
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
