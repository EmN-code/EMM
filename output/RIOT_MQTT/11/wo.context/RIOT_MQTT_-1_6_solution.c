#include <stdio.h>
#include "net/sock/udp.h"  // Network socket for UDP
#include "xtimer.h"       // Required for sleep and timer operations
#include "MQTTClient.h"   // MQTT Client for connecting to the broker

// Function prototypes
void connect_to_mqtt_broker(const char *address, int port, MQTTClient *client, Network *network);
void publish_state(MQTTClient *client, const char *topic, const char *message);

// Main function
int main(void) {
    printf("MQTT client starting.\n");

    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    const char *broker_address = "47.102.103.1";
    int broker_port = 1883;
    const char *topic = "device/state";

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    connect_to_mqtt_broker(broker_address, broker_port, &client, &network);

    while (1) {
        const char *state = "work";  // Deciding what state to publish
        publish_state(&client, topic, state);
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
    return 0;
}

void connect_to_mqtt_broker(const char *address, int port, MQTTClient *client, Network *network) {
    printf("Connecting to MQTT broker at %s:%d\n", address, port);

    // Assuming mqttwrite and mqttread are implemented
    network->mqttwrite = NULL;  // Assign appropriate function
    network->mqttread = NULL;   // Assign appropriate function

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc = MQTTConnect(client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect, return code: %d\n", rc);
    }
}

void publish_state(MQTTClient *client, const char *topic, const char *message) {
    MQTTMessage mqtt_message;
    mqtt_message.payload = (void *)message;
    mqtt_message.payloadlen = strlen(message);
    mqtt_message.qos = QOS0;
    mqtt_message.retained = 0;

    int rc = MQTTPublish(client, topic, &mqtt_message);
    if (rc != SUCCESS) {
        printf("Failed to publish message, return code: %d\n", rc);
    }
}