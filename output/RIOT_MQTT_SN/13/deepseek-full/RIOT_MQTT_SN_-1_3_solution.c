
#include <mutex.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <sys/socket.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"

static mutex_t mtx = MUTEX_INIT;
static sock_udp_ep_t gw = { .port = BROKER_PORT, .family = AF_INET };
static emcute_topic_t topic;

void publish_state(void) {
    const char *state = "work"; // or "not work" based on your logic
    int res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish state: %d\n", res);
    }
}

int main(void) {
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };

    if (inet_aton(BROKER_IP, &remote.addr.ipv4_addr) == 0) {
        printf("Invalid broker IP address\n");
        return 1;
    }

    if (sock_udp_create(&gw, &local, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker\n");
        return 1;
    }

    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return 1;
    }

    while (1) {
        mutex_lock(&mtx);
        publish_state();
        mutex_unlock(&mtx);
        xtimer_sleep(5);
    }

    emcute_discon();
    sock_udp_close(&gw);
    return 0;
}
