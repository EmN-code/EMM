#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <timex.h>
#include "net/emcute.h"
#include "MQTTClient.h"

#define MQTT_BROKER_ADDRESS    "47.102.103.1"
#define MQTT_BROKER_PORT       1883
#define CLIENT_ID              "esp32_mqtt_client"
#define TOPIC_NAME             "state"
#define MESSAGE_CONTENT       "work"
#define QOS_LEVEL              1
#define CONNECTION_TIMEOUT     1000

static char sendbuf[100];
static char readbuf[100];

void publish_state(MQTTClient *client, const char *message_content) {
    MQTTMessage message;
    message.qos = QOS_LEVEL;
    message.retained = 0;
    message.payload = (void *)message_content;
    message.payloadlen = strlen(message_content);
    
    int rc = MQTTPublish(client, TOPIC_NAME, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code: %d\n", rc);
    } else {
        printf("Message published successfully\n");
    }
}

int main(void) {
    printf("Starting MQTT client\n");

    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;
    
    NetworkInit(&network);
    MQTTClientInit(&client, &network, CONNECTION_TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, MQTT_BROKER_ADDRESS, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network, return code: %d\n", rc);
        return rc;
    }

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code: %d\n", rc);
        return rc;
    }

    printf("MQTT client connected\n");
    
    while (1) {
        publish_state(&client, MESSAGE_CONTENT);
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}