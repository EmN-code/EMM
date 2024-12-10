#include "net/sock/udp.h"
#include "kernel_types.h"
#include "kernel.h"
#include "xtimer.h"
#include "emcute.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)

static emcute_topic_t topic;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

int main(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to MQTT-SN broker\n");
        return 1;
    }

    topic.name = "state";
    emcute_reg(&topic);

    while (1) {
        const char *message = "work";
        emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0); // Publish message
        xtimer_sleep(PUB_INTERVAL);
    }
    
    return 0;
}