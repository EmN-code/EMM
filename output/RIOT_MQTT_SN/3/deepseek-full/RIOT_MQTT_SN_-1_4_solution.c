
#include <net/sock/udp.h>
#include <mma8x5x_regs.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <mutex.h>
#include <net/emcute.h>
#include <arpa/inet.h>
#include <kernel_defines.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };

static void publish_state(void) {
    static bool state = true;
    const char *msg = state ? MSG_WORK : MSG_NOT_WORK;
    state = !state;

    int res = emcute_pub(&topic, msg, strlen(msg), 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish message: %d\n", res);
    } else {
        printf("Published: %s\n", msg);
    }
}

int main(void) {
    // Initialize the MQTT-SN client
    emcute_init();

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    // Connect to the MQTT-SN broker
    int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        emcute_discon();
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Disconnect from the MQTT-SN broker
    emcute_discon();

    return 0;
}
