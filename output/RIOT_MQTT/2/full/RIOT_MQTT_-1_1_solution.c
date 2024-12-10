#include "thread.h"
#include "xtimer.h"
#include "mqtt.h"
#include "stdio.h"
#include "stdlib.h"
#include "mqtt_client.h"
#include "string.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_TOPIC "state"
#define DEVICE_ID "esp32_device"

/* MQTT buffer sizes */
#define BUFSIZE 128

/* Define MQTT client context */
static MQTTClient client;
static Network network;

/* MQTT message buffers */
static unsigned char sendbuf[BUFSIZE], readbuf[BUFSIZE];

/* Function to publish a message to the broker */
static void publish_message(const char* message) {
    MQTTMessage msg;
    msg.qos = QOS0;
    msg.retained = 0;
    msg.payload = (void*)message;
    msg.payloadlen = strlen(message);
    MQTTPublish(&client, PUBLISH_TOPIC, &msg);
}

/* Function to setup the MQTT client and connect to the broker */
static int mqtt_connect(Network* network, MQTTClient* client) {
    int rc;

    NetworkInit(network);
    rc = NetworkConnect(network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Error: Could not connect to broker\n");
        return rc;
    }

    MQTTClientInit(client, network, 1000, sendbuf, BUFSIZE, readbuf, BUFSIZE);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = DEVICE_ID;

    rc = MQTTConnect(client, &data);
    if (rc != 0) {
        printf("Error: Could not establish MQTT connection\n");
        return rc;
    }

    printf("Connected to MQTT broker at %s:%d\n", BROKER_IP, BROKER_PORT);
    return rc;
}

void *thread_handler(void *arg) {
    (void)arg;
    const char *states[] = {"work", "not work"};
    size_t index = 0;

    while (1) {
        const char *state = states[index % 2];
        printf("Publishing state: %s\n", state);
        publish_message(state);
        xtimer_sleep(5);
        index++;
    }
    return NULL;
}

int main(void) {
    printf("Initializing MQTT client\n");

    thread_t thread;
    char stack[THREAD_STACKSIZE_DEFAULT];

    int rc = mqtt_connect(&network, &client);
    if (rc != 0) {
        return rc;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "mqttpub");

    while (1) {
        // Keep the main thread running
        xtimer_sleep(10);
    }

    return 0;
}