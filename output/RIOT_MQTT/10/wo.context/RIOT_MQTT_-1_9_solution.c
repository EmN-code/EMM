#include <time.h>         // For timing functionality
#include <stdio.h>        // For input and output functions
#include <stdlib.h>       // For standard library functions
#include <string.h>       // For string manipulation functions
#include <stdbool.h>      // For boolean types and values
#include "MQTTClient.h" // Include the MQTTClient.h last

#define ADDRESS     "tcp://47.102.103.1:1883" // MQTT Broker address
#define CLIENTID    "ESP32Client"             // MQTT Client ID
#define TOPIC       "state/topic"            // The topic to publish to
#define QOS         1                         // Quality of Service level
#define TIMEOUT     10000L                    // MQTT Timeout

void publish_state(MQTTClient client) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;
    
    // Create message
    const char* payload = "work"; // Change to "not work" as needed
    pubmsg.payload = (void*)payload;
    pubmsg.payloadlen = strlen(payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    // Publish message
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    printf("Waiting for publication of %s\non topic %s for client with ClientID: %s\n",
           payload, TOPIC, CLIENTID);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    // Create MQTT client
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    // Connect to the MQTT broker
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    // Continuously publish state every 5 seconds
    while (true) {
        publish_state(client);
        sleep(5);
    }

    // Disconnect and cleanup
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}