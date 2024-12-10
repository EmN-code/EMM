#include <ztimer.h>
#include <stddef.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>
#include <msg.h>
#include <paho_mqtt.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define PUB_INTERVAL 5U  // Publish interval in seconds

static char mqtt_client_buffer[128];
static char mqtt_read_buffer[128];

int main(void) {
    // Network Configuration - Placeholder as there seems to be no direct alternative
    static Network network;
    static MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    int rc = 0;
    
    // Initialize MQTT Client
    MQTTClientInit(&client, &network, 1000, mqtt_client_buffer, sizeof(mqtt_client_buffer), mqtt_read_buffer, sizeof(mqtt_read_buffer));

    // MQTT Connection Options
    options.MQTTVersion = 3;
    options.clientID.cstring = "ESP32_Device";

    // Connect to the MQTT Broker
    if ((rc = MQTTConnect(&client, &options)) != 0) {
        printf("Failed to connect, return code: %d\n", rc);
        return rc;
    }

    while (1) {
        // Publish the state message
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)"work";
        message.payloadlen = strlen("work");

        rc = MQTTPublish(&client, "device/state", &message);
        if (rc != 0) {
            printf("Failed to publish, return code: %d\n", rc);
        }

        // Wait before publishing again
        ztimer_sleep(PUB_INTERVAL);
    }

    return 0;
}