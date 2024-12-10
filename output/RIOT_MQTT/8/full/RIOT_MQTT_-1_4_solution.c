#include <arpa/inet.h>
#include <event/thread.h>
#include <ztimer.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <paho_mqtt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <evtimer_mbox.h>
#include <thread.h>
#include <stdio.h>
#include <mbox.h>
#include <stdatomic.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <msg.h>
#include <unistd.h>
#include <net/gcoap.h>
#include <timex.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define BUFFER_SIZE 100
#define PUBLISH_INTERVAL (5U * 1000000U)  // 5 seconds

Network network;
MQTTClient client;
unsigned char sendbuf[BUFFER_SIZE], readbuf[BUFFER_SIZE];

void publish_state(void) {
    char* topic = "state";
    MQTTMessage message;
    char* payload = "work";
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)payload;
    message.payloadlen = strlen(payload);
    
    while (1) {
        if (MQTTPublish(&client, topic, &message) != 0) {
            printf("Publish failed. Retrying...\n");
        } else {
            printf("Published: %s\n", (char*)message.payload);
        }
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
    }
}

int main(void) {
    printf("Starting MQTT publisher...\n");

    // Initialize network and MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, BUFFER_SIZE, readbuf, BUFFER_SIZE);
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32Client";

    // Connect to the network
    while (1) {
        if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
            printf("Network connection failed. Retrying...\n");
            usleep(1000000);  // 1 second
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }

    // Connect to MQTT Broker
    while (1) {
        if (MQTTConnect(&client, &connectData) != 0) {
            printf("MQTT connection failed. Retrying...\n");
            usleep(1000000);  // 1 second
        } else {
            printf("MQTT connected successfully.\n");
            break;
        }
    }

    // Start publishing state
    publish_state();

    return 0;
}