#include <string.h>  // For handling string operations
#include <stdio.h>    // For standard I/O functions
#include <unistd.h>   // For sleep function
#include "MQTTClient.h"  // MQTT client library for connecting and publishing

#define MQTT_ADDRESS   "tcp://47.102.103.1:1883"
#define CLIENT_ID      "ESP32Client"
#define TOPIC          "device/state"
#define PAYLOAD        "work"
#define QOS            1
#define TIMEOUT        10000L

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, MQTT_ADDRESS, CLIENT_ID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }

    while (1) {
        pubmsg.payload = PAYLOAD;
        pubmsg.payloadlen = strlen(PAYLOAD);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;
        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        printf("Waiting for publication of %s\non topic %s for client with ClientID: %s\n",
                PAYLOAD, TOPIC, CLIENT_ID);
        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        printf("Message with delivery token %d delivered\n", token);
        sleep(5);  // Sleep for 5 seconds
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}