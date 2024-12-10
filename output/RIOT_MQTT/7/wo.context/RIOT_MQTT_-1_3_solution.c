#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/sock/tcp.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_MESSAGE "work or not work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static char mqtt_send_buf[128];
static char mqtt_read_buf[128];

static Network network;
static MQTTClient client;
static MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);
    
    if (MQTTPublish(&client, MQTT_TOPIC, &message) != SUCCESS) {
        printf("Failed to publish the message\n");
    }
    else {
        printf("Message published successfully\n");
    }
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, mqtt_send_buf, sizeof(mqtt_send_buf), mqtt_read_buf, sizeof(mqtt_read_buf));

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to network\n");
        return -1;
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32_Client";

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        mqtt_publish();
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);  // Convert microseconds to seconds
    }

    return 0;
}