#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "MQTTClient.h"
#include "net/sock/tcp.h"

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define MQTT_CLIENT_ID      "ESP32Client"
#define MQTT_TOPIC          "device/state"
#define MQTT_PAYLOAD        "work"
#define MQTT_QOS            QOS0
#define COMMAND_TIMEOUT_MS  4000
#define PUBLISH_INTERVAL    5

void publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = 0;
    message.dup = 0;
    message.id = 0;
    message.payload = (void *)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);
    
    int rc = MQTTPublish(client, MQTT_TOPIC, &message);
    if (rc != SUCCESS) {
        printf("Failed to publish message. Error code: %d\n", rc);
    } else {
        printf("Message published successfully\n");
    }
}

int main(void) {
    /* Buffer sizes for MQTT communication */
    unsigned char sendbuf[100], readbuf[100];

    /* Create MQTT Network and Client structures */
    Network network;
    MQTTClient client;

    /* Initialize MQTT Network and Client */
    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 4;
    data.clientID.cstring = MQTT_CLIENT_ID;

    /* Connect to the MQTT broker */
    if (NetworkConnect(&network, MQTT_BROKER_ADDRESS, MQTT_BROKER_PORT) != 0) {
        fprintf(stderr, "Unable to connect to network\n");
        return EXIT_FAILURE;
    }
    if (MQTTConnect(&client, &data) != 0) {
        fprintf(stderr, "Unable to connect to MQTT broker\n");
        return EXIT_FAILURE;
    }

    /* Main loop to publish state every 5 seconds */
    while (1) {
        publish_state(&client);
        sleep(PUBLISH_INTERVAL);
    }

    return 0;
}