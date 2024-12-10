#include <ztimer.h>  // Include the ztimer header file for time management
#include <thread.h>  // Required for RIOT thread operations
#include <mutex.h>   // Required for mutex usage
#include <stdio.h>   // Standard I/O operations
#include <stdlib.h>  // Standard library for memory allocation
#include "MQTTClient.h"  // MQTT-related operations, included last by requirement

// Define relevant constants for the connection
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "device/state"
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

// Define the device state message
const char *state[] = {"work", "not work"};

// Define the MQTT Client
MQTTClient client;

// Timer definition to manage periodic publishing
ztimer_t state_pub_timer;

// Function to publish the state
void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state[0];
    message.payloadlen = strlen(state[0]);

    // Try publishing the message
    if (MQTTPublish(&client, TOPIC, &message) != SUCCESS) {
        printf("Failed to publish MQTT message\n");
    } else {
        printf("Published state: %s\n", (char *)message.payload);
    }
}

// Function to be run as a separate thread managing MQTT connection and loop
void *mqtt_thread(void *arg) {
    Network network;
    NetworkInit(&network);
    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to the MQTT broker\n");
        return NULL;
    }

    // Initialize the MQTT client
    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "esp32device";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    // Connect to MQTT
    if (MQTTConnect(&client, &data) != SUCCESS) {
        printf("Failed to connect MQTT client\n");
        return NULL;
    }

    // Start the periodic publish
    while (1) {
        ztimer_sleep(ZTIMER-MSEC, 5000);  // Sleep for 5 seconds
        publish_state();
    }

    return NULL;
}

// Main function for initial setup and thread creation
int main(void) {
    puts("ESP32 MQTT example started\n");

    thread_create(stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  mqtt_thread, NULL, "mqtt");

    return 0;
}