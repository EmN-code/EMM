#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "msg.h"
#include "xtimer.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define CLIENT_ID "riot-esp32"

static int send_message(const char *message) {
    int rc = 0; // To track return codes
    // Create a client
    MQTTClient client;
    MQTTClient_create(&client, MQTT_BROKER_IP, CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    // Create and configure connection options
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    // Connect the client
    rc = MQTTClient_connect(client, &conn_opts);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return rc;
    }

    // Create a message
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = (void*) message;
    pubmsg.payloadlen = (int)strlen(message);
    pubmsg.qos = 1;
    pubmsg.retained = 0;

    // Publish the message
    MQTTClient_deliveryToken token;
    rc = MQTTClient_publishMessage(client, MQTT_TOPIC, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);

    // Disconnect from the broker
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return rc;
}

int main(void) {
    printf("MQTT Publisher started\n");

    // Send initial state
    send_message("work");

    while (1) {
        // Toggle message between "work" and "not work" every 5 seconds
        static bool state = true;
        const char *message = state ? "work" : "not work";
        state = !state;

        send_message(message);
        xtimer_sleep(5);
    }
    return 0;
}