#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"
#include "emcute.h"

#define EMCUTE_PORT     (1883U)
#define MQTT_BROKER_IP  "47.102.103.1"
#define MQTT_TOPIC      "state"
#define LOOP_INTERVAL   (5U * US_PER_SEC) // 5 seconds

static emcute_topic_t _topic;

typedef struct {
    sock_udp_ep_t gw;
    char state[13]; // Enough to store "work" or "not work"
} mqtt_data_t;

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;

    /* This function is not used but defined to remove the compilation warning */
}

int main(void) {
    mqtt_data_t mqtt_data;
    strcpy(mqtt_data.state, "work");

    _topic.name = MQTT_TOPIC;
    _topic.id = 0;  // Will be set upon registration with the broker

    printf("Connecting to MQTT-SN broker at: %s\n", MQTT_BROKER_IP);
    int res = inet_pton(AF_INET, MQTT_BROKER_IP, &mqtt_data.gw.addr.ipv4);
    if (res != 1) {
        printf("[Error] Could not convert broker IP\n");
        return 1;
    }
    mqtt_data.gw.family = AF_INET;
    mqtt_data.gw.port = EMCUTE_PORT;

    // Run emcute
    emcute_run(EMCUTE_PORT, "esp32_client");

    while (1) {
        printf("Publishing topic: %s, state: %s\n", _topic.name, mqtt_data.state);
        // Code to publish message to the broker using emcute_pub
        int pub_res = emcute_pub(&_topic, mqtt_data.state, strlen(mqtt_data.state), EMCUTE_QOS_0); 

        if (pub_res != EMCUTE_OK) {
            printf("[Error] emcute_pub failed with code %d\n", pub_res);
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}