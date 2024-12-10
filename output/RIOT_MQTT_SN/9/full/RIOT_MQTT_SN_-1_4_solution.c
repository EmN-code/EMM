#include "mma8x5x.h"
#include "arpa/inet.h"
#include "event/thread.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "thread.h"
#include "dht_params.h"
#include "mma8x5x_regs.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "net/gcoap.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"

#define EMCUTE_ID           "uniqueid"
#define EMCUTE_PORT         (1883U)
#define MQTT_BROKER_IP      "47.102.103.1"
#define PUBLISH_INTERVAL    (5U)

#define TOPIC_NAME "state"
#define MESSAGE_WORK "work"
#define MESSAGE_NOT_WORK "not work"

// Buffer for the message payload
static char buf[256];

void on_message(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    // Handle incoming messages from the broker here
}

int main(void) {
    emcute_topic_t topic;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

    // convert the IP address
    int res = inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);
    if (res <= 0) {
        printf("Error: Failed to convert IP address\n");
        return 1;
    }

    // Loop to connect to broker
    while (1) {
        int rc = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if(rc != EMCUTE_OK) {
            printf("Error: Unable to connect to the broker. Retrying...\n");
            sleep(1);
            continue;
        } else {
            printf("Connected to the broker successfully.\n");
            break;
        }
    }

    topic.name = TOPIC_NAME;
    // Loop to register topic
    while (1) {
        int result = emcute_reg(&topic);
        if (result != EMCUTE_OK) {
            printf("Error: Failed to register topic. Retrying...\n");
            sleep(1);
            continue;
        } else {
            printf("Topic registered successfully.\n");
            break;
        }
    }

    // Main publishing loop
    while (1) {
        // Publish "work" state
        int pub_result = emcute_pub(&topic, MESSAGE_WORK, strlen(MESSAGE_WORK), 0);
        if (pub_result != EMCUTE_OK) {
            printf("Error: Publishing failed\n");
        } else {
            printf("Published state: %s\n", MESSAGE_WORK);
        }

        // Wait for the next publish interval
        sleep(PUBLISH_INTERVAL);
    }

    return 0;
}