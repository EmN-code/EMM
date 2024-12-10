#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_PORT 1883
#define PUB_INTERVAL (5U)
#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static MQTTClient client;
static Network network;
static char sendbuf[100], readbuf[100];

static void *publisher_thread(void *arg) {
    (void)arg;
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)"work";
        message.payloadlen = strlen("work");

        printf("Publishing message: %s\n", (char *)message.payload);
        int rc = MQTTPublish(&client, "state", &message);
        if (rc != SUCCESS) {
            printf("Failed to publish message, return code %d\n", rc);
        }

        xtimer_sleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    NetworkInit(&network);
    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to the network, return code %d\n", rc);
        return 1;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 4;
    data.clientID.cstring = "esp32-client";

    rc = MQTTConnect(&client, &data);
    if (rc != SUCCESS) {
        printf("Failed to connect to the MQTT broker, return code %d\n", rc);
        return 1;
    }

    char stack[STACKSIZE];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, 
                  publisher_thread, NULL, "publisher");

    while (1) {
        // main thread does nothing, publisher_thread takes care of publishing
        xtimer_sleep(60);
    }

    return 0;
}