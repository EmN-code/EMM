#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <paho_mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_TOPIC "device/state"
#define PUBLISH_INTERVAL 5

MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

void publish_state(const char *state) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    pubmsg.payload = (char *)state;
    pubmsg.payloadlen = strlen(state);
    pubmsg.qos = 1;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, MQTT_PUBLISH_TOPIC, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
    printf("Message with delivery token %d delivered\n", token);
}

int main(void) {
    int rc;
    const char *state = "work";
    
    MQTTClient_create(&client, MQTT_BROKER_IP, "ESP32_Client",
        MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    while (1) {
        publish_state(state);
        sleep(PUBLISH_INTERVAL);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}