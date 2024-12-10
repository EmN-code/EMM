#include <ztimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <paho-mqtt/MQTTClient.h>
#include <net/sock/udp.h>
#include <string.h>

#define MQTT_BROKER "47.102.103.1"
#define MQTT_PORT 1883
#define MQTT_TOPIC "device/state"
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // 5 seconds

static MQTTClient client;
static Network network;

void mqtt_connect()
{
    int rc = 0;
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
    data.MQTTVersion = 3;
    data.clientID.cstring = "mqtt_riot_device";

    printf("Connecting to MQTT broker: %s\n", MQTT_BROKER);
    rc = MQTTConnect(&client, &data);
    if (rc != SUCCESS) {
        printf("Error connecting: %d\n", rc);
    }
    else {
        printf("Connected to MQTT broker\n");
    }
}

void mqtt_publish(const char* message)
{
    int rc;
    MQTTMessage pubmsg;
    pubmsg.qos = QOS0;
    pubmsg.retained = 0;
    pubmsg.payload = (void*)message;
    pubmsg.payloadlen = strlen(message);

    rc = MQTTPublish(&client, MQTT_TOPIC, &pubmsg);
    if (rc != SUCCESS) {
        printf("Error publishing message: %d\n", rc);
    }
    else {
        printf("Message published\n");
    }
}

int main(void)
{
    network.mqttread = /* user-defined sock reading function */;
    network.mqttwrite = /* user-defined sock writing function */;

    mqtt_connect();

    char *state_message = "work or not work";

    while (1) {
        mqtt_publish(state_message);
        ztimer_sleep(ZTIMER_SEC, PUBLISH_INTERVAL);
    }
    return 0;
}