#include <xtimer.h> // Include for timer utilities
#include <net/protnum.h> // Include for network protocol numbers
#include <string.h>   // Include for string operations
#include "MQTTClient.h" // Must be the last included header

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "ESP32Client"
#define TOPIC "device/state"
#define QOS 0
#define TIMEOUT 10000L // in milliseconds

typedef struct {
    Network network;
    MQTTClient client;
} mqtt_client_info_t;

static mqtt_client_info_t mqtt_client_info;

void connect_and_publish(void) {
    int rc = 0;
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = CLIENT_ID;

    printf("Connecting to %s:%d...\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
    rc = NetworkConnect(&mqtt_client_info.network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Error: Unable to connect to MQTT broker (Error code: %d)\n", rc);
        return;
    }

    rc = MQTTConnect(&mqtt_client_info.client, &connect_data);
    if (rc != SUCCESS) {
        printf("Error: Unable to connect to MQTT client (Error code: %d)\n", rc);
        return;
    }

    printf("Successfully connected to %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    while (1) {
        MQTTMessage message;
        char payload[20];
        snprintf(payload, sizeof(payload), "{\"state\": \"%s\"}", "work");

        message.qos = QOS;
        message.retained = 0;
        message.payload = (void *)payload;
        message.payloadlen = strlen(payload);

        rc = MQTTPublish(&mqtt_client_info.client, TOPIC, &message);
        if (rc != SUCCESS) {
            printf("Error: Unable to publish message (Error code: %d)\n", rc);
        } else {
            printf("Message published to topic %s\n", TOPIC);
        }

        xtimer_sleep(5);
    }
}

int main(void) {
    NetworkInit(&mqtt_client_info.network);
    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&mqtt_client_info.client, &mqtt_client_info.network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    connect_and_publish();
    return 0;
}