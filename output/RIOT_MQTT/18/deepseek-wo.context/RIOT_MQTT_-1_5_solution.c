
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void mqtt_publish(MQTTClient *client, const char *topic, const char *payload)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    MQTTPublish(client, topic, &message);
}

int main(void)
{
    MQTTClient client;
    Network network;
    unsigned char sendbuf[256], readbuf[256];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_esp32";
    data.keepAliveInterval = 30;
    data.cleansession = 1;

    rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        mqtt_publish(&client, TOPIC, PAYLOAD);
        xtimer_usleep(INTERVAL);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
