#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/mqtt.h>
#include <net/network.h>

#define MQTT_BROKER_IP    "47.102.103.1"
#define MQTT_BROKER_PORT  1883
#define MQTT_PUB_TOPIC    "device/state"
#define MQTT_MSG_WORK     "work"
#define MQTT_MSG_NOWORK   "not work"

static char mqtt_sendbuf[128];
static char mqtt_readbuf[128];
MQTTClient client;
Network network;

int main(void)
{
    int rc;
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, mqtt_sendbuf, sizeof(mqtt_sendbuf), mqtt_readbuf, sizeof(mqtt_readbuf));

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Connected to network successfully.\n");
        break;
    }

    while (1) {
        printf("Connecting to MQTT broker...\n");
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Connected to MQTT broker successfully.\n");

        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (char*)MQTT_MSG_WORK;
        message.payloadlen = strlen(MQTT_MSG_WORK);

        while (1) {
            rc = MQTTPublish(&client, MQTT_PUB_TOPIC, &message);
            if (rc != 0) {
                printf("Failed to publish message to MQTT. Reconnecting and retrying...\n");
                MQTTDisconnect(&client);
                NetworkDisconnect(&network);
                break;
            }
            printf("Published state: %s\n", MQTT_MSG_WORK);
            xtimer_sleep(5);
        }
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
    return 0;
}