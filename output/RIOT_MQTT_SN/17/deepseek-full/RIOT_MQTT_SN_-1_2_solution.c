
#include <net/emcute.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <net/sock/udp.h>
#include <sys/socket.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"

static emcute_topic_t topic;
static sock_udp_ep_t gw_ep = SOCK_IPV4_EP_ANY;

void publish_state(void) {
    const char *msg = MESSAGE;
    unsigned flags = 0;

    int res = emcute_pub(&topic, msg, strlen(msg), flags);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Message published successfully\n");
    }
}

int main(void) {
    // Initialize the UDP endpoint for the gateway
    gw_ep.port = EMQX_SERVER_PORT;
    gw_ep.family = AF_INET;
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw_ep.addr.ipv4);

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&gw_ep, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN gateway: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        emcute_discon();
        return 1;
    }

    // Run the emCute message receiver
    emcute_run(EMQX_SERVER_PORT, CLIENT_ID);

    // Publish state every five seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    // Disconnect from the gateway
    emcute_discon();

    return 0;
}
