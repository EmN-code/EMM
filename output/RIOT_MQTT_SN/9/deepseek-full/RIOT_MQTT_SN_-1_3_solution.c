
#include "event/thread.h"
#include "thread.h"
#include "net/sock/util.h"
#include "net/ipv6/addr.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "sys/socket.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"
#define STACK_SIZE 1024
#define PRIORITY THREAD_PRIORITY_MAIN - 1

static char stack[STACK_SIZE];
static emcute_topic_t topic;
static sock_udp_ep_t gw_ep = SOCK_IPV6_EP_ANY;

void publish_state(void) {
    while (1) {
        int res = emcute_pub(&topic, STATE_MESSAGE, sizeof(STATE_MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish state: %d\n", res);
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    // Initialize the UDP socket
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;
    local.port = 12345;
    remote.port = EMQX_SERVER_PORT;

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    gw_ep.port = EMQX_SERVER_PORT;
    gw_ep.family = AF_INET;
    gw_ep.netif = SOCK_ADDR_ANY_NETIF;
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &gw_ep.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    if (emcute_con(&gw_ep, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN gateway\n");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return 1;
    }

    // Create a thread to publish state every five seconds
    thread_create(stack, sizeof(stack), PRIORITY, 0, (thread_task_func_t)publish_state, NULL, "publisher");

    return 0;
}
