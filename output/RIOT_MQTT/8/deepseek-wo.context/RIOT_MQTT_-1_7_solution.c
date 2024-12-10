
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t sock;

int mqtt_read(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_read(&sock, buffer, len, 0);
}

int mqtt_write(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_write(&sock, buffer, len, 0);
}

int main(void)
{
    Network network;
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize the network
    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 30000, NULL, 0, NULL, 0);

    // Connect to the MQTT broker
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_esp32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    printf("Connected to MQTT broker\n");

    // Publish state every 5 seconds
    while (1) {
        xtimer_usleep_until(&last_wakeup, MQTT_MESSAGE_INTERVAL);
        last_wakeup = xtimer_now();

        MQTTMessage message;
        message.qos = 0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void *)MQTT_MESSAGE;
        message.payloadlen = strlen(MQTT_MESSAGE);

        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Published message: %s\n", MQTT_MESSAGE);
        }
    }

    return 0;
}
