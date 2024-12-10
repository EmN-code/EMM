
#include <stdio.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static void mqtt_connect(MQTTClient *client, Network *network)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    int rc = MQTTConnect(client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
    } else {
        printf("Connected to MQTT broker\n");
    }
}

static void mqtt_publish(MQTTClient *client, const char *topic, const char *payload)
{
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    int rc = MQTTPublish(client, topic, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Published message: %s\n", payload);
    }
}

int main(void)
{
    Network network;
    MQTTClient client;
    unsigned char sendbuf[256], readbuf[256];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network, return code %d\n", rc);
        return 1;
    }

    mqtt_connect(&client, &network);

    while (1) {
        mqtt_publish(&client, MQTT_TOPIC, MQTT_PAYLOAD);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
