#include "ztimer.h"
#include "unistd.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/ipv6/addr.h"
#include "net/emcute.h"

#define EMCUTE_PORT     (1883U)
#define MQTT_BROKER_IP  "47.102.103.1"
#define PUB_TOPIC_NAME  "status"
#define DEVICE_STATE    "work"
#define DEVICE_STATE_ALT "not work"
#define PUB_INTERVAL    (5U)

static emcute_topic_t pub_topic;
static sock_udp_ep_t remote;

int main(void)
{
    // Convert broker IP address
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: Could not convert MQTT broker IP address");
        return 1;
    }
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = EMCUTE_PORT;

    // Register topic
    pub_topic.name = PUB_TOPIC_NAME;
    if (emcute_reg(&pub_topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Connect to the broker
    while (1) {
        int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            puts("Connected to broker");
            break;
        }
        else {
            puts("Failed to connect, retrying in 1 second...");
            sleep(1);
        }
    }

    // Publish loop
    while (1) {
        const char *state = DEVICE_STATE;
        int res = emcute_pub(&pub_topic, state, strlen(state), 0);
        if (res != EMCUTE_OK) {
            puts("Error: unable to publish");
            continue;
        }
        printf("Published state: %s\n", state);

        sleep(PUB_INTERVAL);
    }

    return 0;
}
