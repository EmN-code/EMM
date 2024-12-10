
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <net/sock/udp.h>
#include <thread.h>
#include <mma8x5x_params.h>
#include <net/gcoap.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <event/thread.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"

static emcute_topic_t topic;
static sock_udp_ep_t gw_ep = SOCK_IPV4_EP_ANY;

void publish_state(void) {
    static bool state = false;
    const char *msg = state ? MSG_WORK : MSG_NOT_WORK;
    state = !state;

    int res = emcute_pub(&topic, msg, strlen(msg), 0);
    if (res != EMCUTE_OK) {
        puts("Error publishing message");
    }
}

int main(void) {
    // Initialize the MQTT-SN client
    emcute_init();

    // Set up the gateway endpoint
    gw_ep.port = EMQX_SERVER_PORT;
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw_ep.addr.ipv4);

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&gw_ep, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN gateway");
        return 1;
    }

    // Register the topic
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
