
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <net/sock/udp.h>
#include <thread.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <event/thread.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC "state"
#define MESSAGE "work"

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMQX_SERVER_PORT };

void publish_state(void) {
    while (1) {
        int res = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Error publishing message: %d\n", res);
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    // Initialize the MQTT-SN client
    emcute_init();

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN gateway: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    // Create a thread to publish state every 5 seconds
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, (thread_task_func_t)publish_state, NULL, "publisher");

    return 0;
}