#include <thread.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define BUFFER_SIZE 100
#define INTERVAL 5  // in seconds

static char stack[THREAD_STACKSIZE_MAIN];

// Message publication function
void* publish_thread(void *arg) {
    (void)arg;
    MQTTClient client;
    Network network;
    unsigned char sendbuf[BUFFER_SIZE], readbuf[BUFFER_SIZE];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc;

    // Initialize the MQTT client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, BUFFER_SIZE, readbuf, BUFFER_SIZE);

    // Setup the network parameters
    char addr[256];
    snprintf(addr, sizeof(addr), "%s:%d", BROKER_IP, BROKER_PORT);

    rc = NetworkConnect(&network, addr);
    if (rc != 0) {
        printf("Failed to connect to network, return code %d\n", rc);
        return NULL;
    }

    // Configure the MQTT connection data
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_ESP32_Client";

    // Connect to the MQTT broker
    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Failed to connect to broker, return code %d\n", rc);
        return NULL;
    }
    printf("Connected to broker\n");

    // Prepare the message structure
    MQTTMessage message;
    message.qos = 1;
    message.retained = 0;
    message.payload = "work";
    message.payloadlen = strlen("work");

    while (1) {
        // Publish message
        if ((rc = MQTTPublish(&client, "state", &message)) != 0) {
            printf("Publish failed, return code %d\n", rc);
        } else {
            printf("Message published: %s\n", (char *)message.payload);
        }
        // Wait for the next interval
        sleep(INTERVAL);
    }

    return NULL;
}

int main(void) {
    // Start the message publishing thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_thread, NULL, "publish_thread");

    return 0;
}
