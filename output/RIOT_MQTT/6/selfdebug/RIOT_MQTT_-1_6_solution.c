#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <net/mqtt.h>
#include <net/gnrc.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define CLIENT_ID "esp32_client"
#define STATUS_MSG "work"
#define SLEEP_DURATION (5U * 1000000U) // 5 seconds in microseconds

static char mqtt_sendbuf[128];
static char mqtt_readbuf[128];

static mqtt_client_t mqtt_client;
static Network network;

void setup_network(void) {
    NetworkInit(&network);
    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to network.\n");
        exit(1);
    }
}

void setup_mqtt_client(void) {
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTClientInit(&mqtt_client, &network, 3000, mqtt_sendbuf, sizeof(mqtt_sendbuf), mqtt_readbuf, sizeof(mqtt_readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    int rc = MQTTConnect(&mqtt_client, &connectData);
    if (rc != SUCCESS) {
        printf("MQTT connect failed with error code %d.\n", rc);
        exit(1);
    }
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)STATUS_MSG;
    message.payloadlen = strlen(STATUS_MSG);
    int rc = MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);

    if (rc != SUCCESS) {
        printf("Error while publishing message: %d\n", rc);
    }
}

int main(void) {
    setup_network();
    setup_mqtt_client();

    while (1) {
        publish_state();
        xtimer_usleep(SLEEP_DURATION);
    }

    return 0;
}