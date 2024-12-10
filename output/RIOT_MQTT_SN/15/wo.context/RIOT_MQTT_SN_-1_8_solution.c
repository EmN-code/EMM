//
// This program connects an ESP32 running RIOT OS to an MQTT-SN broker and periodically publishes a message.
// It uses the emcute library to maintain efficiency and reduce energy consumption.
//
// Key functionality:
// - Connects to an MQTT-SN broker specified by an IP address and port.
// - Publishes a "state" message every five seconds indicating "work" or "not work".
// - Includes error handling for network connection and message publishing.
//

#include <string.h>
#include <stdio.h>
#include <net/ipv4/addr.h>  // This import solves inet_pton error
#include <xtimer.h>
#include <emcute.h>

#define MQTT_BROKER_IP     "47.102.103.1"
#define MQTT_BROKER_PORT   1883
#define PUB_INTERVAL       (5U * 1000000U)   // Publish every 5 seconds

static char stack[THREAD_STACKSIZE_DEFAULT];

// Callback function for publishing, now addressed to avoid compile errors
static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Published to topic '%s': %.*s\n", topic->name, (int)len, (char *)data);
}

int main(void) {
    emcute_sub_t sub;
    emcute_topic_t t;
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = MQTT_BROKER_PORT
    };

    // Convert MQTT broker IP address
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("ERROR: invalid IPv4 address\n");
        return 1;
    }

    emcute_run(MQTT_BROKER_PORT, "esp32_client");

    // Try to connect to the MQTT-SN broker
    while (1) {
        int con_result = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (con_result == EMCUTE_OK) {
            printf("Connected to MQTT-SN broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
            break;
        }
        else {
            printf("Connection to MQTT-SN broker failed, retrying in 1 second...\n");
            xtimer_sleep(1);
        }
    }

    // Register the topic
    t.name = "device/state";
    while (1) {
        int reg_result = emcute_reg(&t);
        if (reg_result == EMCUTE_OK) {
            printf("Registered topic: %s\n", t.name);
            break;
        }
        else {
            printf("Topic registration failed, retrying...\n");
            xtimer_sleep(1);
        }
    }

    // Periodically publish the state
    const char *state = "work or not work";
    while (1) {
        int pub_result = emcute_pub(&t, state, strlen(state), EMCUTE_QOS_0);
        if (pub_result != EMCUTE_OK) {
            printf("Publish failed\n");
        }
        else {
            on_pub(&t, state, strlen(state));
        }
        xtimer_usleep(PUB_INTERVAL);
    }
    return 0;
}
