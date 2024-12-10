#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "thread.h"
#include "sema.h"
#include "msg.h"
#include "mqtt.h"
#include "socket.h"
#include "xtimer.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32-client"
#define TOPIC "device/state"

char mqtt_thread_stack[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[8];

static void *mqtt_thread(void *arg) {
    (void)arg;
    MQTTClient client;
    Network network;
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 2000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to network: %d\n", rc);
        return NULL;
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return NULL;
    }

    char *message = "work";
    MQTTMessage mqtt_message;
    mqtt_message.qos = QOS1;
    mqtt_message.retained = 0;
    mqtt_message.payload = (void *) message;
    mqtt_message.payloadlen = strlen(message);

    while (1) {
        rc = MQTTPublish(&client, TOPIC, &mqtt_message);
        if (rc != SUCCESS) {
            printf("Failed to publish message: %d\n", rc);
        } else {
            printf("Message published: %s\n", (char *)mqtt_message.payload);
        }

        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    msg_init_queue(msg_queue, 8);
    xtimer_sleep(1);
    return 0;
}