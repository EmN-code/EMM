
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/tcp.h"
#include "net/mqtt_client.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static MQTTClient mqtt_client;
static Network network;

static void mqtt_connect(void)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "riot_esp32";

    int rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("MQTT connect failed\n");
    } else {
        printf("MQTT connected\n");
    }
}

static void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    int rc = MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed\n");
    } else {
        printf("MQTT message published\n");
    }
}

int main(void)
{
    printf("Starting MQTT example\n");

    /* Initialize the network */
    NetworkInit(&network);

    /* Initialize the MQTT client */
    MQTTClientInit(&mqtt_client, &network, 1000, NULL, 0, NULL, 0);

    /* Connect to the MQTT broker */
    mqtt_connect();

    /* Publish a message every 5 seconds */
    while (1) {
        mqtt_publish();
        xtimer_usleep(MQTT_INTERVAL);
    }

    return 0;
}
