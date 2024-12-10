
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <paho_mqtt.h>
#include <ztimer.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work or not work"
#define QOS 0
#define RETAIN false

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char recvbuf[100];

void connect_to_broker(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_ESP32";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    printf("Connected to MQTT broker\n");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = RETAIN;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    int rc = MQTTPublish(&client, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Published: %s\n", PAYLOAD);
    }
}

int main(void) {
    connect_to_broker();

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}
