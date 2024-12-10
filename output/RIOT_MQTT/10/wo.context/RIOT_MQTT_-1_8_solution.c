#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <xtimer.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883

static int _publish_status(MQTTClient* client) {
    MQTTMessage message;
    char payload[20];
    snprintf(payload, sizeof(payload), "work");

    message.qos = QOS0;
    message.retained = 0;
    message.payload = payload;
    message.payloadlen = strlen(payload);

    int rc = MQTTPublish(client, "status", &message);
    if (rc != SUCCESS) {
        printf("Failed to publish message: %d\n", rc);
        return rc;
    }

    printf("Published: %s\n", payload);
    return SUCCESS;
}

static void *mqtt_thread(void *arg) {
    (void)arg;

    MQTTClient client;
    Network network;

    NetworkInit(&network);

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != SUCCESS) {
        printf("Network connection failed\n");
        return NULL;
    }

    MQTTClientInit(&client, &network, 3000, NULL, 0, NULL, 0);

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("MQTT connection failed\n");
        return NULL;
    }

    while (1) {
        _publish_status(&client);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_DEFAULT];

    thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  mqtt_thread, NULL, "mqtt_thread");

    return 0;
}