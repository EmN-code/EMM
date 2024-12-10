
#include <thread.h>
#include <stdlib.h>
#include <mutex.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mma8x5x_regs.h>
#include <sys/socket.h>
#include <event/thread.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };

void publish_state(void) {
    const char *state = "work";  // or "not work" based on your logic
    int res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish state: %d\n", res);
    } else {
        printf("State published: %s\n", state);
    }
}

void *emcute_thread(void *arg) {
    (void)arg;

    // Initialize the gateway endpoint
    if (inet_aton(MQTT_SN_BROKER_IP, &gw.addr.ipv4) == 0) {
        printf("Error: invalid broker IP address\n");
        return NULL;
    }

    // Connect to the MQTT-SN broker
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return NULL;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return NULL;
    }

    // Run the emCute message receiver
    emcute_run(MQTT_SN_BROKER_PORT, CLIENT_ID);

    return NULL;
}

int main(void) {
    // Create a thread for emCute
    thread_create(
        thread_stack(0),
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        emcute_thread,
        NULL,
        "emcute_thread"
    );

    // Publish state every 5 seconds
    while (1) {
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
        publish_state();
    }

    return 0;
}
