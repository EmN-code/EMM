#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "mqtt_client.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define MQTT_PUBLISH_INTERVAL 5
#define MQTT_BUFFER_SIZE    128
#define MQTT_CLIENT_ID      "esp32_mqtt_client"
#define MQTT_TOPIC          "device/state"
#define MQTT_MSG_WORK       "work"
#define MQTT_MSG_NOT_WORK   "not work"

static MQTTClient client;
static Network network;
static unsigned char sendbuf[MQTT_BUFFER_SIZE], readbuf[MQTT_BUFFER_SIZE];

static void mqtt_publish_state(bool work_state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)(work_state ? MQTT_MSG_WORK : MQTT_MSG_NOT_WORK);
    message.payloadlen = strlen((char *)message.payload);

    MQTTPublish(&client, MQTT_TOPIC, &message);
}

int main(void) {
    sock_udp_ep_t remote;
    int result;

    memset(&remote, 0, sizeof(remote));
    result = inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);
    if (result == 0) {
        printf("Invalid IP address format\n");
        return 1;
    } else if (result < 0) {
        perror("inet_pton failed");
        return 1;
    }

    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;

    MQTTClientInit(&client, &network, 1000, sendbuf, MQTT_BUFFER_SIZE, readbuf, MQTT_BUFFER_SIZE);

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    if (MQTTConnect(&client, &connectData) < 0) {
        printf("MQTT connect failed\n");
        return 1;
    }

    bool work_state = true;
    while (1) {
        mqtt_publish_state(work_state);
        sleep(MQTT_PUBLISH_INTERVAL);
        work_state = !work_state;
    }

    return 0;
}