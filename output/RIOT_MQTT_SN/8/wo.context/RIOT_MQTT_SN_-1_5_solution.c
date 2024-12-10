#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "ztimer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define PUB_INTERVAL 5

static emcute_topic_t topic;

/**
 * @brief Callback function to initialize emcute
 * 
 * This function must be implemented by the application, to set the correct
 * transport layer for emcute.
 */
void emcute_init(void)
{
    // Configuring the emcute transport layer perhaps
}

/**
 * @brief Function to connect to the MQTT-SN broker
 */
int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    inet_pton(AF_INET6, BROKER_IP, &gw.addr.ipv6);
    
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Could not connect to broker");
        return 1;
    }
    return 0;
}

/**
 * @brief Publish a message to the MQTT-SN broker
 */
void publish_state(void) {
    const char *message = "work"; // Change this as necessary
    unsigned flags = EMCUTE_QOS_0;

    if (emcute_pub(&topic, message, strlen(message), flags) != EMCUTE_OK) {
        puts("Publish failed");
    } else {
        puts("Publish successful");
    }
}

int main(void) {
    topic.name = TOPIC_NAME;

    if (connect_to_broker() == 0) {
        while (1) {
            publish_state();
            ztimer_sleep(ZTIMER_SEC, PUB_INTERVAL);
        }
    }

    return 0;
}