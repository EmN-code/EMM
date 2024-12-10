#include "mma8x5x.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "stdio.h"
#include "dht_params.h"
#include "mma8x5x_regs.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "net/gcoap.h"
#include "mma8x5x_params.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC_NAME "device_status"
#define STATE_MESSAGE "work or not work"
#define DELAY 5

enum { EMCUTE_ID = 0 };

static emcute_topic_t topic;
static sock_udp_ep_t gw;

int main(void) {
    int res;
    
    // Convert broker IP to network byte order
    gw.family = AF_INET;
    gw.port = MQTT_BROKER_PORT;
    
    while (1) {
        // Attempt to convert the IP address
        res = inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);
        if (res <= 0) {
            puts("Error: Invalid MQTT broker IP address.");
            continue;
        }
        break;
    }

    // Try to connect to the MQTT-SN broker
    while (1) {
        res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res != EMCUTE_OK) {
            printf("Error: Unable to connect to broker. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Successfully connected to broker.\n");
        break;
    }

    // Prepare the topic
    topic.name = TOPIC_NAME;
    topic.id = EMCUTE_ID;
    
    // Register the topic
    while (1) {
        res = emcute_reg(&topic);
        if (res != EMCUTE_OK) {
            printf("Error: Unable to register topic. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Topic successfully registered.\n");
        break;
    }

    // Periodically publish the state
    while (1) {
        res = emcute_pub(&topic, STATE_MESSAGE, strlen(STATE_MESSAGE), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: Unable to publish message.\n");
        } else {
            printf("Message published: %s\n", STATE_MESSAGE);
        }
        xtimer_sleep(DELAY);
    }

    return 0;
}