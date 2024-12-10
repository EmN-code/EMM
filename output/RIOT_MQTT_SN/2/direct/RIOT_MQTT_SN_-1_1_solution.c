#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_PORT           (1883U)
#define MQTT_TOPIC           "device/state"
#define MQTT_MSG_WORK        "work"
#define MQTT_MSG_NOT_WORK    "not work"
#define LOOP_INTERVAL_SEC     (5U)

static emcute_sock_t sock;
static char *server_addr = "47.102.103.1";

static int publish_state(void) {
    char *message = (xtimer_now_usec() / 1000000) % 10 < 5 ? MQTT_MSG_WORK : MQTT_MSG_NOT_WORK;

    emcute_topic_t topic;
    topic.name = MQTT_TOPIC;

    return emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
}

int main(void) {
    sock = emcute_sock_init();

    ipv6_addr_t addr;
    if (ipv6_addr_from_str(&addr, server_addr) == NULL) {
        printf("Error: unable to parse server address\n");
        return -1;
    }

    uint16_t port = EMCUTE_PORT;
    if (emcute_con(&sock, &addr, port, NULL, 0, NULL) != EMCUTE_OK) {
        printf("Error: unable to connect to server\n");
        return -1;
    }

    while (1) {
        if (publish_state() != EMCUTE_OK) {
            printf("Error: unable to publish message\n");
        }
        xtimer_sleep(LOOP_INTERVAL_SEC);
    }

    return 0;
}