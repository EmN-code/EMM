#include <stdio.h>
#include <string.h>
#include "sock_udp.h"
#include "MQTTClient.h"  // MQTTClient.h should be the last header included as per the instruction

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define MQTT_PUBLISH_INTERVAL (5U)
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define MESSAGE_WORK "work"
#define MESSAGE_NOT_WORK "not work"

static sock_udp_ep_t remote;
static MQTTClient client;
static char sendbuf[100];
static char readbuf[100];

void publish_state(void) {
    static int toggle = 0;
    char *message = toggle ? MESSAGE_WORK : MESSAGE_NOT_WORK;
    MQTTMessage mqtt_msg;
    mqtt_msg.payload = message;
    mqtt_msg.payloadlen = strlen(message);
    mqtt_msg.qos = QOS0;
    mqtt_msg.retained = 0;
    MQTTPublish(&client, TOPIC, &mqtt_msg);
    toggle = !toggle;
}

int main(void) {
    printf("[MQTT] ESP32 MQTT Client Started\n");

    // Initialize UDP communication endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize the MQTT client
    Network network;
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Establish TCP connection
    NetworkConnect(&network, SERVER_IP, SERVER_PORT);

    // Prepare the connection data and connect to the MQTT broker
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.willFlag = 0;
    data.MQTTVersion = 3;
    data.clientID.cstring = CLIENT_ID;
    data.keepAliveInterval = 10;
    data.cleansession = 1;
    MQTTConnect(&client, &data);

    // Publishing loop
    while (1) {
        publish_state();
        sleep(MQTT_PUBLISH_INTERVAL);
    }

    return 0;
}