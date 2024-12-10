#include <stdio.h>
#include "msg.h"
#include "net/sock/udp.h"
#include "emcute.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "device/state"

static emcute_topic_t topic;
static char message[] = "state: work";

void on_pub(void) {
    /* Publish message every 5 seconds */
    while (1) {
        int res = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        xtimer_usleep(5 * 1000000);
    }
}

int main(void) {
    puts("Start MQTT-SN client");

    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    emcute_sub_t subscription = { .topic.name = TOPIC_NAME, .cb = NULL };

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Could not connect to gateway");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Unable to register topic");
        return 1;
    }

    on_pub();

    return 0;
}