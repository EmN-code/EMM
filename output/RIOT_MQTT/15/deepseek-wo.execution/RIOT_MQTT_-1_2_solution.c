
#include <net/emcute.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include "MQTTClient.h"  // Ensure this is the last header file to be included

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"

static MQTTClient mqttClient;
static Network network;
static unsigned char sendbuf[200];
static unsigned char readbuf[200];

void mqtt_publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTPublish(&mqttClient, MQTT_TOPIC, &message);
}

int main(void) {
    // Initialize the network
    NetworkInit(&network);

    // Connect to the MQTT broker
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Initialize the MQTT client
    MQTTClientInit(&mqttClient, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the MQTT broker
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_ESP32";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    if (MQTTConnect(&mqttClient, &data) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish_state();
        vTaskDelay(pdMS_TO_TICKS(5000));  // Delay for 5 seconds
    }

    // Disconnect from the MQTT broker
    MQTTDisconnect(&mqttClient);
    NetworkDisconnect(&network);

    return 0;
}
