
#include <net/emcute.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <mma8x5x_regs.h>
#include <net/sock/udp.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <sys/socket.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"

static emcute_topic_t topic;

void publish_state(void) {
    const char *state = "work"; // or "not work" based on your logic
    int res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish state: %d\n", res);
    } else {
        printf("State published: %s\n", state);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .port = EMQX_SERVER_PORT };
    int res = inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);
    if (res != 1) {
        printf("Failed to convert IP address\n");
        return 1;
    }

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return 1;
    }

    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        emcute_discon();
        return 1;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    emcute_discon();
    return 0;
}
