// Description:
// This program connects an ESP32 device to an MQTT-SN broker using the emcute library. 
// It publishes a message indicating the device state every five seconds. The program is 
// carefully structured to minimize energy consumption, leveraging the efficient emcute 
// library for MQTT-SN communication.

#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define NODE_ID "esp32"
#define TOPIC_NAME "device/state"

// Message states
static const char *state_working = "work";
static const char *state_not_working = "not_work";

static emcute_topic_t topic;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    printf("Publish to topic received\n");
}

static int connect_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    int res = inet_pton(AF_INET, BROKER_ADDRESS, &gw.addr.ipv4);

    if (res != 1) {
        puts("Error: Unable to parse broker address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: Unable to connect to broker");
        return 1;
    }

    printf("MQTT-SN: Connected to broker %s\n", BROKER_ADDRESS);
    return 0;
}

static int publish_state(const char *state) {
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    emcute_reg(&topic);

    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error: Unable to publish");
        return 1;
    }

    printf("Published state: %s\n", state);
    return 0;
}

int main(void) {
    if (connect_broker() != 0) {
        return 1;
    }

    while (1) {
        publish_state(state_working);
        xtimer_sleep(5);
        publish_state(state_not_working);
        xtimer_sleep(5);
    }

    return 0;
}