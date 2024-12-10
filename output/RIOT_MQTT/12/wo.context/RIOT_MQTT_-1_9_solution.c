#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "kernel_defines.h"
#include "kernel.h"
#include "msg.h"
#include "net/af.h"
#include "net/sock/udp.h"
#include "net/sock/udp.h"
#include "paho-mqtt/MQTTClient.h"

#define MQTT_BROKER_IP       "47.102.103.1"
#define MQTT_BROKER_PORT     1883
#define MQTT_PUB_TOPIC       "state"
#define MQTT_PUB_MSG_WORK    "work"
#define MQTT_PUB_MSG_NOWORK  "not work"
#define MQTT_PUB_INTERVAL    (5U)
#define MQTT_CLIENT_ID       "esp32_client"

static char send_buffer[256];
static char receive_buffer[256];

/* MQTT client configuration */
MQTTClient client;
Network network;
MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;

static void mqtt_publish_state(MQTTClient* client) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;

    static bool state = false;
    if (state) {
        message.payload = (void*)MQTT_PUB_MSG_WORK;
        message.payloadlen = strlen(MQTT_PUB_MSG_WORK);
    } else {
        message.payload = (void*)MQTT_PUB_MSG_NOWORK;
        message.payloadlen = strlen(MQTT_PUB_MSG_NOWORK);
    }

    MQTTPublish(client, MQTT_PUB_TOPIC, &message);
    state = !state;
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, send_buffer, sizeof(send_buffer), receive_buffer, sizeof(receive_buffer));

    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = MQTT_CLIENT_ID;

    inet_pton(AF_INET, MQTT_BROKER_IP, &network.ipstack.ip_buffer);
    network.ipstack.ip_len = sizeof(network.ipstack.ip_buffer);
    network.ipstack.sock.fd = sock_udp_create(&network.ipstack.sock, NULL, NULL, 0);

    while (MQTTConnect(&client, &connect_data) != MQTT_SUCCESS) {
        puts("Unable to connect to MQTT broker, retrying...");
        xtimer_sleep(1);
    }

    puts("Connected to MQTT broker");

    while (1) {
        mqtt_publish_state(&client);
        xtimer_sleep(MQTT_PUB_INTERVAL);
    }

    return 0;
}