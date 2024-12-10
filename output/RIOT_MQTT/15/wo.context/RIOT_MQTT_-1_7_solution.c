#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "msg.h"
#include "od.h"
#include "thread.h"
#include "xtimer.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID        "esp32_client"
#define TOPIC            "device/state"
#define KEEP_ALIVE_INTERVAL 20
#define MESSAGE_INTERVAL (5U * US_PER_SEC)

static char mqtt_client_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t mqtt_msg_queue[8];

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

static const char* state = "work";  // Default state

void mqtt_publish(const char* message) {
    MQTTMessage mqtt_message;
    mqtt_message.qos = QOS0;
    mqtt_message.retained = 0;
    mqtt_message.payload = (void*)message;
    mqtt_message.payloadlen = strlen(message);

    int rc = MQTTPublish(&client, TOPIC, &mqtt_message);
    if (rc == FAILURE) {
        printf("Failed to publish message\n");
    }
    else {
        printf("Published: %s\n", message);
    }
}

int main(void) {
    printf("RIOT MQTT Client
");

    msg_init_queue(mqtt_msg_queue, ARRAY_SIZE(mqtt_msg_queue));

    NetworkInit(&network);
  
    while (1) {
        if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != SUCCESS) {
            printf("Network connection failed. Retrying\n");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("Network connected\n");
            break;
        }
    }

    MQTTClientInit(&client, &network, KEEP_ALIVE_INTERVAL * 1000, sendbuf, sizeof(sendbuf), readbuf, siz
    int rc;  
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = CLIENT_ID;

    while (1) {
        if ((rc = MQTTConnect(&client, &connect_data)) != SUCCESS) {
            printf("MQTT connection failed: %d\n", rc);
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("MQTT connected\n");
            break;
        }
    }

    while (1) {
        mqtt_publish(state);
        xtimer_sleep(MESSAGE_INTERVAL);
    }

    return 0;
}