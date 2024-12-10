#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/mqtt.h>
#include <net/sock/udp.h>
#include <timex.h>
#include <xtimer.h>

static const char *mqtt_broker_addr = "47.102.103.1";
static const char *topic = "state/status";
static const int port = 1883;
static const char *publish_message = "work";
static const int publish_interval = 5;

static MQTTClient client;
static Network network;

void publish_state(void)
{
    MQTTMessage message;
    message.qos = QOS1;
    message.retained = 0;
    message.payload = (void *)publish_message;
    message.payloadlen = strlen(publish_message);

    int rc = MQTTPublish(&client, topic, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Message published: %s\n", publish_message);
    }
}

int main(void)
{
    int rc;
    unsigned char sendbuf[128], readbuf[128];

    NetworkInit(&network);
    rc = NetworkConnect(&network, mqtt_broker_addr, port);
    if (rc != 0) {
        printf("Failed to connect to broker\n");
        return rc;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = "ESP32_Client";

    rc = MQTTConnect(&client, &connect_data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return rc;
    }

    while (1) {
        publish_state();
        xtimer_sleep(publish_interval);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}