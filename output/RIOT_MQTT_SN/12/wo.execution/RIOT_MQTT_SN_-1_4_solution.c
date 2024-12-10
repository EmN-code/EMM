#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <thread.h>
#include <arpa/inet.h>

// Define the address and port of the MQTT-SN broker
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

// Define the topic and message to publish
#define TOPIC_NAME "device/state"
#define WORK_MESSAGE "work"
#define NOT_WORK_MESSAGE "not work"

// Define the interval for publishing messages
#define PUBLISH_INTERVAL (5U)  // in seconds

static emcute_topic_t topic;

// Function to connect to the MQTT-SN broker
static int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse broker IP");
        return -1;
    }
    return emcute_con(&gw, true, NULL, NULL, 0, 0);
}

// Function to publish a message to the MQTT-SN broker
static int publish_state(const char *message) {
    return emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
}

int main(void) {
    // Run emcute with the predefined port and a client ID
    emcute_run(BROKER_PORT, "esp32_client");

    // Connect to the broker
    if (connect_to_broker() != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Periodically publish messages
    int state = 0;  // 0 for not work, 1 for work
    while (1) {
        if (state == 0) {
            publish_state(NOT_WORK_MESSAGE);
            state = 1;
        } else {
            publish_state(WORK_MESSAGE);
            state = 0;
        }
        sleep(PUBLISH_INTERVAL);
    }

    return 0;
}