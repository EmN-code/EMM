
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/sock/util.h>
#include <net/ipv4/addr.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS QOS0
#define MQTT_RETAIN 0
#define MQTT_YIELD_TIME 1000
#define MQTT_PUBLISH_INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[200];
static unsigned char readbuf[200];

void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
}

int main(void)
{
    printf("Starting MQTT client...\n");

    /* Initialize the MQTT client */
    MQTTClientInit(&mqtt_client, &mqtt_network, MQTT_YIELD_TIME, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    /* Connect to the MQTT broker */
    int rc = NetworkConnect(&mqtt_network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return 1;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_mqtt_client";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("MQTT connect failed: %d\n", rc);
        return 1;
    }

    printf("Connected to MQTT broker\n");

    /* Publish a message every 5 seconds */
    while (1) {
        mqtt_publish();
        xtimer_usleep(MQTT_PUBLISH_INTERVAL);
    }

    /* Disconnect from the MQTT broker */
    MQTTDisconnect(&mqtt_client);
    NetworkDisconnect(&mqtt_network);

    return 0;
}
