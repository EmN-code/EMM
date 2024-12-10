#include <stdio.h> // for printf
#include <stdlib.h> // for standard library
#include "MQTTClient.h" // Ensure this is included last as per requirement

#define MQTT_ADDRESS "tcp://47.102.103.1:1883"
#define CLIENT_ID "RIOT_ESP32_Client"
#define PUBLISH_TOPIC "state_topic"
#define QOS 1
#define TIMEOUT 10000L

static MQTTClient client;

void publish_state() {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    pubmsg.payload = (void *)"work";
    pubmsg.payloadlen = 4;
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    rc = MQTTClient_publishMessage(client, PUBLISH_TOPIC, &pubmsg, &token);

    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to publish message, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for up to %d seconds for publication of %s\n"
           "on topic %s for client with ClientID: %s\n",
           (int)(TIMEOUT/1000), "work", PUBLISH_TOPIC, CLIENT_ID);

    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);

    printf("Message with delivery token %d delivered\n", token);
}

int main() {
    int rc;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    MQTTClient_create(&client, MQTT_ADDRESS, CLIENT_ID,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    // Publish periodically every 5 seconds
    while (1) {
        publish_state();
        sleep(5);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return rc;
}