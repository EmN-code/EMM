#include <unistd.h>
#include "ztimer.h"
#include "evtimer.h"
#include "net/mqttsn.h"
#include "net/gcoap.h"
#include "net/af.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    int rc;

    // Initialize network
    NetworkInit(&network);
    
    // Connect to the network with retry on failure
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            usleep(1000000); // Sleep for a second before retrying
            continue;
        }
        printf("Network connected successfully\n");
        break;
    }

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 4; // Use MQTT 3.1.1
    connectData.clientID.cstring = "esp32_client";

    // Connect to the MQTT broker with retry on failure
    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed, retrying...\n");
            usleep(1000000); // Sleep for a second before retrying
            continue;
        }
        printf("MQTT connected successfully\n");
        break;
    }

    // Publish state every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        char payload[] = "work";
        message.payload = payload;
        message.payloadlen = strlen(payload);

        rc = MQTTPublish(&client, "state", &message);
        if (rc != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Message published\n");
        }

        usleep(PUBLISH_INTERVAL); // Wait for the next cycle
    }

    return 0;
}