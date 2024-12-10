#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>  // Ensure 'MQTTClient.h' is the last inclusion if needed.

// MQTT client properties
#define ADDRESS     "tcp://47.102.103.1:1883"
#define CLIENTID    "ESP32Client"
#define TOPIC       "device/state"
#define PAYLOAD1    "work"
#define PAYLOAD2    "not work"
#define QOS         1
#define TIMEOUT     10000L

MQTTClient client;

// Function to initialize MQTT client and perform connection setup
void init_mqtt_client() {
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
}

// Function to publish message to the broker
void publish_message(const char* payload) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    pubmsg.payload = (void*)payload;
    pubmsg.payloadlen = strlen(payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
}

int main() {
    // Initialize MQTT client
    init_mqtt_client();

    while (true) {
        // Publish "work" state
        publish_message(PAYLOAD1);

        // Wait for 5 seconds
        sleep(5);

        // Toggle between "work" and "not work"
        publish_message(PAYLOAD2);

        // Wait for 5 seconds
        sleep(5);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return 0;
}