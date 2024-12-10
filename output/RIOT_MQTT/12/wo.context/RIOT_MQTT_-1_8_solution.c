#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "net/sock/udp.h"
#include "net/mqttsn.h"
#include "MQTTClient.h"

#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define MQTT_KEEP_ALIVE 60

static mqtt_client_t mqtt_client;
static sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SERVER_PORT };

void publish_state(const char *state) {
    mqtt_message_t message;
    memset(&message, 0, sizeof(message));
    message.qos = MQTT_QOS_0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    if (mqtt_publish(&mqtt_client, "state", &message) != SOCK_OK) {
        printf("Error: unable to publish\n");
    } else {
        printf("Published: %s\n", state);
    }
}

int main(void) {
    printf("Starting MQTT client\n");
    sock_udp_create(&mqtt_client.sock, NULL, NULL, 0);
    mqtt_init(&mqtt_client, &gw, NULL, 0);

    if (mqtt_connect(&mqtt_client, NULL, MQTT_KEEP_ALIVE, false, NULL, NULL, 0, NULL, 0) != SOCK_OK) {
        printf("Error: unable to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        publish_state("work");
        sleep(5);
        publish_state("not work");
        sleep(5);
    }

    return 0;
}