#include <stdbool.h>
#include <stddef.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <mqtt/paho_mqtt.h>
#include <MQTTClient.h>

#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define MQTT_CLIENT_ID "riot_esp32_client"
#define MQTT_PUBLISH_TOPIC "device/state"
#define MQTT_PUBLISH_MSG "work"
#define MQTT_QOS 1
#define MQTT_TIMEOUT 10000L
#define SLEEP_DURATION 5

static MQTTClient client;
static Network network;

static void publish_state(bool state) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = 0;
    message.payload = (void*)(state ? "work" : "not work");
    message.payloadlen = strlen(message.payload);

    MQTTPublish(&client, MQTT_PUBLISH_TOPIC, &message);
}

int main(void) {
    int ret;

    NetworkInit(&network);
    ret = NetworkConnect(&network, MQTT_SERVER_IP, MQTT_SERVER_PORT);
    if (ret != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    MQTTClientInit(&client, &network, MQTT_TIMEOUT, NULL, 0, NULL, 0);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = MQTT_CLIENT_ID;

    ret = MQTTConnect(&client, &data);
    if (ret != 0) {
        puts("Failed to connect MQTT client");
        return 1;
    }

    bool state = true;
    while (true) {
        publish_state(state);
        state = !state;  // Toggle state
        sleep(SLEEP_DURATION);
    }

    return 0;
}