// Solution to fix the issues in the RIOT program for ESP32 using emcute library
// to publish a message every five seconds to a MQTT-SN broker.

// Make sure you have the correct include headers
#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

// Define the stack for the `main` thread
#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

// Define the MQTT broker's IP address and port
#define BROKER_IP           "47.102.103.1"
#define BROKER_PORT         (1883)

// Define the publish state
#define PUB_STATE           "work"
#define PUB_TOPIC           "state"

static emcute_sub_t _subscriptions[1];
static char _stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t _topics[1];

// MQTT-SN message callback (currently not used, but necessary for attaching to emcute)
static void on_mqtt_sn_msg(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

// Function to initialize emcute application
void emcute_init(void) {
    memset(_subscriptions, 0, (sizeof(emcute_sub_t) * ARRAY_SIZE(_subscriptions)));
    emcute_run(BROKER_PORT, "esp32");
}

// Connection handler to MQTT-SN broker
int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6 };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, BROKER_IP);
    gw.port = BROKER_PORT;
  
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Failed to connect to broker");
        return 1;
    }
    return 0;
}

// Publish the state message to the MQTT-SN broker
void publish_state(void) {
    _topics[0].name = PUB_TOPIC;
    if (emcute_reg(&_topics[0]) != EMCUTE_OK) {
        puts("Unable to register topic");
        return;
    }

    if (emcute_pub(&_topics[0], PUB_STATE, strlen(PUB_STATE), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Unable to publish data");
        return;
    }
}

int main(void) {
    // Initialize the main message queue
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize emcute
    emcute_init();

    // Connect to the broker
    if (connect_to_broker() != 0) {
        return 1; // Exit if unable to connect
    }

    while (1) {
        publish_state();

        // Wait 5 seconds before publishing again
        xtimer_sleep(5);
    }

    return 0;
}