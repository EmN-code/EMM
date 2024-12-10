#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "xtimer.h"
#include "net/emcute.h"

#define SLEEP_INTERVAL (5 * US_PER_SEC)  // Publish every 5 seconds
#define MQTT_BROKER "47.102.103.1"
#define MQTT_PORT 1883
#define MQTT_TOPIC "state"

static emcute_sock_t sock;

static void mqtt_publish(const char *topic, const char *data) {
    emcute_pub_t pub;
    pub.topic.name = topic;
    pub.data = (void *)data;
    pub.data_len = strlen(data);
    pub.qos = EMCUTE_QOS_0;
    pub.retain = 0;
    
    if (emcute_pub(&pub) != EMCUTE_OK) {
        printf("[MQTT] Publish failed\n");
    }
    else {
        printf("[MQTT] Published: %s => %s\n", topic, data);
    }
}

static int mqtt_connect(void) {
    sock.host = MQTT_BROKER;
    sock.port = MQTT_PORT;
    
    int res = emcute_con(&sock, true, NULL, NULL);
    if (res != EMCUTE_OK) {
        printf("[MQTT] Connection failed\n");
        return -1;
    }
    printf("[MQTT] Connected to broker %s on port %d\n", MQTT_BROKER, MQTT_PORT);
    return 0;
}

int main(void) {
    printf("[MQTT] Starting MQTT publisher example with RIOT\n");
    if (mqtt_connect() != 0) {
        return -1;
    }

    while (1) {
        mqtt_publish(MQTT_TOPIC, "work");
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}