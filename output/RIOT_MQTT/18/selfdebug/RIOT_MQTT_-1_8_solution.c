#include <stdio.h>  // Standard I/O operations
#include <stdlib.h> // Standard library
#include <string.h> // String operations
#include "paho-mqtt/MQTTClient.h"

#define MQTT_BROKER_ADDRESS "tcp://47.102.103.1:1883"
#define CLIENT_ID           "ESP32_Client"
#define TOPIC               "device/state"
#define DEFAULT_MESSAGE     "work"
#define DEFAULT_NOT_WORK_MESSAGE "not work"
#define QOS                 1
#define TIMEOUT             10000L

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    // Initialize MQTT client
    MQTTClient_create(&client, MQTT_BROKER_ADDRESS, CLIENT_ID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);

    // Set connection options
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    // Connect to the MQTT broker
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    char *payload = DEFAULT_MESSAGE;
    while (1) {
        // Create payload
        pubmsg.payload = payload;
        pubmsg.payloadlen = strlen(payload);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

        // Publish the message
        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        printf("Waiting for up to %d seconds for publication of %s\n"
               "on topic %s for client with ClientID: %s\n",
               (int)(TIMEOUT/1000), payload, TOPIC, CLIENT_ID);
        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        printf("Message with delivery token %d delivered\n", token);

        // Simulate change of state
        payload = (strcmp(payload, DEFAULT_MESSAGE) == 0) ? DEFAULT_NOT_WORK_MESSAGE : DEFAULT_MESSAGE;

        // Sleep for 5 seconds
        sleep(5);
    }

    // Disconnect the MQTT client
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return rc;
}