#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "net/ipv6/addr.h"
#include "net/sock/tcp.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_TOPIC "state"
#define INTERVAL (5 * US_PER_SEC)

static char sendbuf[100];
static char readbuf[100];

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    char *mqtt_message = "work or not work";

    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "client_id";    

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Connection to MQTT broker failed\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)mqtt_message;
        message.payloadlen = strlen(mqtt_message);

        if (MQTTPublish(&client, MQTT_PUBLISH_TOPIC, &message) != SUCCESS) {
            printf("Error while publishing message\n");
        } else {
            printf("Message published: %s\n", mqtt_message);
        }

        xtimer_sleep(5);
    }

    NetworkDisconnect(&network);

    return 0;
}
