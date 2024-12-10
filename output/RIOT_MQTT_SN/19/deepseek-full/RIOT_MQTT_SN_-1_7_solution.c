
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <ztimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work or not work"
#define QOS 0
#define TIMEOUT 10000L

MQTTClient client;

void publish_message(void) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = (void *)PAYLOAD;
    pubmsg.payloadlen = sizeof(PAYLOAD) - 1;
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(&client, TOPIC, &pubmsg, &token);
    MQTTClient_waitForCompletion(&client, token, TIMEOUT);
    printf("Message '%s' with delivery token %d delivered\n", PAYLOAD, token);
}

int main(void) {
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, "tcp://47.102.103.1:1883", "RIOT_MQTT_CLIENT", MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(&client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    while (1) {
        publish_message();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    MQTTClient_disconnect(&client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
