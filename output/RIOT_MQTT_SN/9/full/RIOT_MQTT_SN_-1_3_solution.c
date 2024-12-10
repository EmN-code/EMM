#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "thread.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_pub"
#define TOPIC_NAME "state"

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };

void connect_to_broker(void) {
    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Connected to broker successfully\n");
            break;
        } else {
            printf("Connection failed, retrying...\n");
            ztimer_sleep(ZTIMER_SEC, 1);
        }
    }
}

void publish_state(const char *state) {
    while (1) {
        int res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        if (res == EMCUTE_OK) {
            printf("Published: %s\n", state);
            break;
        } else {
            printf("Publish failed, retrying...\n");
            ztimer_sleep(ZTIMER_SEC, 1);
        }
    }
}

int main(void) {
    gw.netif = SOCK_ADDR_ANY_NETIF;

    char addr_str[16];
    inet_pton(AF_INET6, SERVER_ADDR, addr_str);
    memcpy(gw.addr.ipv6, addr_str, sizeof(gw.addr.ipv6));

    emcute_run(SERVER_PORT, CLIENT_ID);

    connect_to_broker();

    topic.name = TOPIC_NAME;
    while (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic, retrying...\n");
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    while (1) {
        static const char *state = "work";
        publish_state(state);
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}