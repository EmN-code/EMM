#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "string.h"

#define EMCUTE_ID         (1U)
#define EMCUTE_PRIO       (THREAD_PRIORITY_MAIN - 1)

static sock_udp_ep_t remote = { .family = AF_INET6, .port = 1883 }; // Using IPv6 for the address family

static const char *mprv_topic = "state";
static const char *message = "work or not work";

void mqtt_connect(void) {
    while (1) {
        int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Connected to MQTT-SN broker\n");
            break;
        }
        else {
            printf("Failed to connect, retrying...\n");
            xtimer_sleep(1);
        }
    }
}

void mqtt_publish(emcute_topic_t *topic) {
    int res = emcute_pub(topic, message, strlen(message), 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish message\n");
    }
}

int main(void) {
    inet_pton(AF_INET6, "::ffff:2f66:671", &remote.addr.ipv6); // Converting IPv4 address to IPv6-Mapped

    emcute_topic_t topic;
    strcpy(topic.name, mprv_topic); // Ensure the length of topic.name is within limits

    mqtt_connect();

    emcute_reg(&topic);

    while (1) {
        mqtt_publish(&topic);
        xtimer_sleep(5);
    }

    return 0;
}