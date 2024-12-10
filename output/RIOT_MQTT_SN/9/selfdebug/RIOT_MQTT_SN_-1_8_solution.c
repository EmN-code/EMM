#include "xtimer.h"
#include "string.h"
#include "net/sock/udp.h"
#include "net/mqttsn.h"
#include "emcute.h"
#include <stdbool.h>

#define EMCUTE_PORT     (1883U)
#define MQTT_TOPIC      "state"

static sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT };

void publish_state(void) {
    emcute_topic_t topic;
    topic.name = MQTT_TOPIC;

    char *message = "work";

    if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    }
}

int main(void) {
    puts("MQTT-SN publisher starting...");

    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    while(1) {
        publish_state();
        xtimer_sleep(5);
    }

    emcute_discon();
    return 0;
}