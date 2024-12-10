#include "thread.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "event/thread.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "test/topic"

static char stack[THREAD_STACKSIZE_MAIN];

// Declare the topic
static emcute_topic_t topic;

void *publish_thread(void *arg) {
    (void)arg;
    const char *message = "work";
    
    while (1) {
        int res = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            // Error handling here
            continue;
        }
        // Emulate 5-second delay
        for (volatile int i = 0; i < 1000000; ++i);
    }
    return NULL;
}

int main(void) {
    emcute_config_t config;
    sock_udp_ep_t gw = { .family = AF_INET6 };

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, publish_thread, NULL, "pub_thread");
    
    // Configure the MQTT-SN broker address
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_IP);
    gw.port = MQTT_BROKER_PORT;

    // Start the emcute
    int res = emcute_init(&config);
    if (res != EMCUTE_OK) {
        // Error handling for initialization
        return 1;
    }

    // Set the topic information
    topic.name = MQTT_TOPIC;
    topic.id = 0;  // Assume topic is registered with id of 0

    // Connect to the broker
    if ((res = emcute_con(&gw, true)) != EMCUTE_OK) {
        // Connection error handling
        return 1;
    }

    while (1) {
        thread_yield();
    }
    
    return 0;
}
