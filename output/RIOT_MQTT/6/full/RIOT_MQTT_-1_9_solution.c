#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "example/topic"
#define CLIENT_ID "RIOT-MQTT-Client"

static char sendbuf[100];
static char readbuf[100];

void publish_work_state(MQTTClient* client) {
    static bool work_state = false;
    char message[20];

    if (work_state) {
        snprintf(message, sizeof(message), "work");
    } else {
        snprintf(message, sizeof(message), "not work");
    }
    work_state = !work_state;

    MQTTMessage mqtt_message;
    mqtt_message.qos = QOS0;
    mqtt_message.retained = 0;
    mqtt_message.dup = 0;
    mqtt_message.payload = message;
    mqtt_message.payloadlen = strlen(message);

    int rc;
    while ((rc = MQTTPublish(client, TOPIC, &mqtt_message)) != 0) {
        printf("Error publishing message: retrying in 1 second\n");
        xtimer_sleep(1);
    }

    printf("Published message: %s\n", message);
}

int main(void) {
    Network network;
    MQTTClient client;

    NetworkInit(&network);

    int rc;
    while ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
        printf("Network connection failed, retrying in 1 second\n");
        xtimer_sleep(1);
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    while ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("MQTT connection failed, retrying in 1 second\n");
        xtimer_sleep(1);
    }

    printf("Successfully connected to MQTT broker\n");

    while (1) {
        publish_work_state(&client);
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}