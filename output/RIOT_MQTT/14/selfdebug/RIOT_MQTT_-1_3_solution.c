#include "xtimer.h"  // For using xtimer_sleep
#include "net/ipv4/addr.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "stdio.h"

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID   "riot-esp32-client"
#define MQTT_TOPIC       "state"
#define MQTT_PAYLOAD     "work"
#define MQTT_PAYLOADLEN  4  // Length of 'work'

static MQTTClient client;
static Network network;

void mqtt_init(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
  
    int rc = 0;
    char addr[100];
    snprintf(addr, sizeof(addr), "%s:%d", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    if ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
        printf("Failed to connect to network, return code %d\n", rc);
        return;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = MQTT_CLIENT_ID;
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    if ((rc = MQTTConnect(&client, &data)) != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return;
    }
    printf("Connected to MQTT broker\n");
}

void mqtt_publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)MQTT_PAYLOAD;
    message.payloadlen = MQTT_PAYLOADLEN;

    if (MQTTPublish(&client, MQTT_TOPIC, &message) != SUCCESS) {
        printf("Failed to publish MQTT message\n");
    } else {
        printf("MQTT message published\n");
    }
}

int main(void) {
    mqtt_init();
    while(1) {
        mqtt_publish_state();
        xtimer_sleep(5);  // Publish every 5 seconds
    }
    return 0;
}