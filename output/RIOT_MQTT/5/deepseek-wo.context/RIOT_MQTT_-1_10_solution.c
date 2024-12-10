
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/tcp.h"
#include "net/mqtt_client.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static MQTTClient mqtt_client;
static Network mqtt_network;

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&mqtt_client, TOPIC, &message);
}

int main(void) {
    unsigned char sendbuf[256];
    unsigned char readbuf[256];

    MQTTClientInit(&mqtt_client, &mqtt_network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&mqtt_network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "riot_esp32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        mqtt_publish();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
