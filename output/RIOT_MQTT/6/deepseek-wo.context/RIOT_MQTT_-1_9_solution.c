
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static MQTTClient client;
static Network network;

void network_init(void) {
    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;
}

int main(void) {
    printf("Starting MQTT example\n");

    network_init();
    NetworkConnect(&network, SERVER_IP, SERVER_PORT);

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_client";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    if (MQTTConnect(&client, &data) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        MQTTMessage message;
        message.qos = 0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = PAYLOAD_LEN;

        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Published message: %s\n", PAYLOAD);
        }

        xtimer_usleep(INTERVAL);
    }

    return 0;
}
