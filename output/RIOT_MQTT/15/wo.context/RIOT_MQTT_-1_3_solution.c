#include <stdio.h>
#include <string.h>
#include "MQTTClient.h" // Make sure this is included last.

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL 5

// Initialize necessary networking variables
Network network;
MQTTClient client;
MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
MQTTMessage message;

void publish_state() {
    int rc;

    msg.payload = (void *)"work";
    msg.payloadlen = strlen("work");
    msg.qos = QOS0;
    msg.retained = 0;

    while(1) {
        rc = MQTTPublish(&client, "state", &message);
        if(rc != SUCCESS) {
            printf("Publish failed with error code %d, retrying in 1 second\n", rc);
            xtimer_sleep(1); // Wait before retrying
            continue;
        }

        printf("Published successfully\n");
        xtimer_sleep(PUBLISH_INTERVAL); // Wait 5 seconds before republishing
    }
}

int main(void) {
    int rc;

    printf("Initializing network\n");
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    // Connect to the network
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != SUCCESS) {
            printf("Network connection failed with error code %d, retrying in 1 second\n", rc);
            xtimer_sleep(1);
            continue;
        }
        break;
    }

    printf("Connecting to MQTT broker\n");
    connectData.MQTTVersion = 3;
    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != SUCCESS) {
            printf("MQTT connection failed with error code %d, retrying in 1 second\n", rc);
            xtimer_sleep(1);
            continue;
        }
        break;
    }

    printf("Connection successful, starting publish loop\n");
    publish_state();

    return 0;
}