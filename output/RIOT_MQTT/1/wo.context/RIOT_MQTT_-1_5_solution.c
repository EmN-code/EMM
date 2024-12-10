#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <net/mqttsn.h>
#include <MQTTClient.h>

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "RIOT_ESP32_Client"
#define TOPIC "state"
#define QOS 0
#define MESSAGE "work or not work"
#define DELAY 5000000  // in microseconds (5 seconds)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[80], readbuf[80];

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void *) MESSAGE;
    message.payloadlen = strlen(MESSAGE);

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    // Initialize network
    NetworkConnect(&network, SERVER_ADDRESS, SERVER_PORT);

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTConnect(&client, &(MQTTPacket_connectData) {
        .clientID.cstring = CLIENT_ID,
        .keepAliveInterval = 20,
        .cleansession = 1
    });

    while (1) {
        publish_state();
        xtimer_usleep(DELAY);
    }
    return 0;
}