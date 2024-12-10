#include "sys/socket.h"
#include "stdlib.h"
#include "stdio.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "mma8x5x_regs.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL_SEC 5
#define TOPIC_NAME "device/state"

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };

int main(void) {
    printf("Connecting to MQTT-SN broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse broker address");
        return 1;
    }
    int res;

    // Try connecting to the broker
    while (1) {
        res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Connected successfully to the broker!\n");
            break;
        } else {
            printf("Failed to connect to broker, retrying in 1s...\n");
            sleep(1);  // Retry after a second
        }
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    while (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic, retrying in 1s...\n");
        sleep(1);
    }

    // Publish loop
    while (1) {
        const char *state = (rand() % 2) ? "work" : "not work";
        res = emcute_pub(&topic, state, strlen(state), 0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish, retrying...\n");
        } else {
            printf("Published state: %s\n", state);
        }
        sleep(PUBLISH_INTERVAL_SEC);
    }

    return 0;
}