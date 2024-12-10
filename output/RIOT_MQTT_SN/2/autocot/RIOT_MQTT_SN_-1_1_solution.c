#include "timex.h"
#include "thread.h"
#include "emcute.h"
#include "xtimer.h"

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC_NAME "device/state"
#define PUBLISH_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

static emcute_socket_t sock;
static emcute_topic_t topic;

void publish_state(void) {
    const char *state = "work";  // Example state
    emcute_msg_t msg;

    msg.data = (void *)state;
    msg.len = strlen(state);
    msg.type = EMCUTE_MSG_TYPE_PUBLISH;
    msg.flags = EMCUTE_MSG_FLAG_RETAIN;

    if (emcute_pub(&sock, &topic, &msg) != EMCUTE_OK) {
        printf("Error publishing message\n");
    }
    else {
        printf("State published: %s\n", state);
    }
}

int main(void) {
    // Initialize the hardware and networking interface here
    printf("Connecting to MQTT-SN broker at %s:%d\n", MQTT_BROKER_ADDRESS, MQTT_BROKER_PORT);

    // Setup the socket and network configurations for Emcute
    sock.net = NULL;
    sock.unixfd = -1;
    emcute_network_init();
    emcute_sock(&sock);

    // Set the topic details
    topic.name = MQTT_TOPIC_NAME;
    topic.id = 0;  // Will be set by the broker

    emcute_connect(&sock, MQTT_BROKER_ADDRESS, MQTT_BROKER_PORT);

    // The main loop to publish messages
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);  // Sleep for 5 seconds
    }

    return 0;
}