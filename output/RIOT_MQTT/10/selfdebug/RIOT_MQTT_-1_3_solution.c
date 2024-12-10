#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xtimer.h>
#include <pthread.h>
#include "net/sock/tcp.h"
#include "net/mqttsn.h"

#define MQTT_MSG "work or not work"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "RIOT_ESP32_Client"
#define MQTT_TOPIC "state"
#define MQTT_KEEPALIVE 30
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

// MQTT client structure
static MQTTClient client;
static Network network;
static unsigned char sendbuf[80], readbuf[80];

void *mqtt_publish_thread(void *arg) {
    (void)arg;
    int rc;
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)MQTT_MSG;
    message.payloadlen = strlen(MQTT_MSG);
    while (1) {
        // Publish the message
        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != SUCCESS) {
            printf("Error publishing message: %d\n", rc);
        }

        // Wait 5 seconds
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    printf("Starting MQTT client\n");
    int rc;

    // Initialize the network
    NetworkInit(&network);
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Error connecting to MQTT broker: %d\n", rc);
        return -1;
    }

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;
    connectData.keepAliveInterval = MQTT_KEEPALIVE;

    // Connect to the MQTT broker
    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Error connecting to MQTT broker: %d\n", rc);
        return -1;
    }

    // Create a thread to publish messages
    pthread_t pub_thread;
    pthread_create(&pub_thread, NULL, mqtt_publish_thread, NULL);

    // Wait for the publishing thread to complete
    pthread_join(pub_thread, NULL);

    return 0;
}
