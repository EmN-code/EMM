
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <sys/socket.h>
#include "paho_mqtt.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define KEEP_ALIVE 60
#define COMMAND_TIMEOUT_MS 30000
#define SEND_BUF_SIZE 100
#define READ_BUF_SIZE 100

static MQTTClient client;
static Network network;
static unsigned char sendbuf[SEND_BUF_SIZE];
static unsigned char readbuf[READ_BUF_SIZE];

void connect_to_broker(void) {
    int rc;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = KEEP_ALIVE;
    options.cleansession = 1;
    options.clientID.cstring = "riot_client";

    printf("Connecting to %s:%d\n", BROKER_IP, BROKER_PORT);
    NetworkInit(&network);
    rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network, return code %d\n", rc);
        return;
    }

    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, sendbuf, SEND_BUF_SIZE, readbuf, READ_BUF_SIZE);
    rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return;
    }
    printf("Connected to MQTT broker\n");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    int rc = MQTTPublish(&client, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Published message: %s\n", PAYLOAD);
    }
}

int main(void) {
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
