#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUB_TOPIC "device/state"
#define KEEP_ALIVE_INTERVAL 20

static const char *state_msg = "work";

timer_t delay_timer;

void mqtt_publish(MQTTClient *client) {
    MQTTMessage message;
    message.qos = QOS1;
    message.retained = 0;
    message.payload = (void *)state_msg;
    message.payloadlen = strlen(state_msg);

    int rc = MQTTPublish(client, MQTT_PUB_TOPIC, &message);
    if (rc != 0) {
        printf("Error publishing message: %d\n", rc);
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    int rc;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, KEEP_ALIVE_INTERVAL * 1000,
                   NULL, 0, NULL, 0);

    char *serverAddress = MQTT_BROKER_IP;
    if ((rc = NetworkConnect(&network, serverAddress, MQTT_BROKER_PORT)) != 0) {
        printf("Error connecting to broker: %d\n", rc);
        return 1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.keepAliveInterval = KEEP_ALIVE_INTERVAL;
    connectData.cleansession = 1;

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("MQTT connect failed: %d\n", rc);
        return 1;
    }

    while (1) {
        mqtt_publish(&client);
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}