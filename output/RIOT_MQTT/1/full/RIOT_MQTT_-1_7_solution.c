#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdatomic.h>
#include <arpa/inet.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <ztimer.h>
#include <timex.h>
#include <mutex.h>
#include <msg.h>
#include <thread.h>
#include <dht.h>
#include <dht_params.h>
#include <xtimer.h>
#include <time_units.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP    "47.102.103.1"
#define MQTT_BROKER_PORT  1883
#define MQTT_CLIENT_ID    "esp32_client"
#define MQTT_TOPIC        "esp32/state"
#define MQTT_MSG          "work"
#define PUBLISH_INTERVAL  (5U * 1000U * 1000U) // 5 seconds

static MQTTClient client;

static void mqtt_publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = MQTT_MSG;
    message.payloadlen = strlen(MQTT_MSG);
    int rc = MQTTPublish(&client, MQTT_TOPIC, &message);
    if (rc != SUCCESS) {
        printf("Error publishing message: %d\n", rc);
    } else {
        printf("Message published\n");
    }
}

int main(void) {
    Network network;
    int rc = NetworkInit(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Error initializing network: %d\n", rc);
        return -1;
    }

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = MQTT_CLIENT_ID;

    rc = MQTTConnect(&client, &connect_data);
    if (rc != SUCCESS) {
        printf("Error connecting to broker: %d\n", rc);
        return -1;
    }

    while (1) {
        mqtt_publish_state();
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
    }

    return 0;
}