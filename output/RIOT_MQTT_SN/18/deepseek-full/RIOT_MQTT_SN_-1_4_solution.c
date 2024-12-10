
#include <event/thread.h>
#include <net/emcute.h>
#include <thread.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <mma8x5x_regs.h>
#include <arpa/inet.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"
#define PUBLISH_INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;
static sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };

void publish_state(void) {
    while (1) {
        int res = emcute_pub(&topic, STATE_MESSAGE, sizeof(STATE_MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish state: %d\n", res);
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }
}

int main(void) {
    /* Convert IP address string to binary form */
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Invalid IP address\n");
        return 1;
    }

    /* Connect to the MQTT-SN broker */
    int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to broker: %d\n", res);
        return 1;
    }

    /* Register the topic */
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    /* Start the thread to publish state every 5 seconds */
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)publish_state, NULL, "publish_state");

    return 0;
}
