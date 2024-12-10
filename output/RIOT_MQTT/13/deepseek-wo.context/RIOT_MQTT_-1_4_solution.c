
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void mqtt_connect(MQTTClient *client, Network *network)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "riot_esp32";

    int rc = MQTTConnect(client, &options);
    if (rc != 0) {
        printf("MQTT connect failed\n");
        return;
    }
    printf("MQTT connected\n");
}

static void mqtt_publish(MQTTClient *client)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = MQTT_MESSAGE_LEN;

    int rc = MQTTPublish(client, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed\n");
        return;
    }
    printf("MQTT message published\n");
}

int main(void)
{
    Network network;
    MQTTClient client;

    // Initialize network
    NetworkInit(&network);
    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Network connect failed\n");
        return 1;
    }
    printf("Network connected\n");

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    // Connect to MQTT broker
    mqtt_connect(&client, &network);

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish(&client);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
