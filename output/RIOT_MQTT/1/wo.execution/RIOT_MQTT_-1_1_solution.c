#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "stdio.h"
#include "mqtt.h"
#include "MQTTClient.h"

#define BROKER_IP   "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

static void mqtt_pub_handler(void)
{
    MQTTMessage message;
    char* topic = "device/state";
    char* payload = "work";

    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)payload;
    message.payloadlen = strlen(payload);

    printf("Publishing: %s to topic: %s\n", payload, topic);
    MQTTPublish(&client, topic, &message);
}

int main(void)
{
    printf("Connecting to MQTT Broker\n");
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    MQTTStartTask(&client);

    printf("Connection successful\n");

    while (1) {
        mqtt_pub_handler();
        xtimer_sleep(5);
    }

    return 0;
}
