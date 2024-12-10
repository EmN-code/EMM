#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_device"
#define MQTT_STATE_MSG "work"
#define MSG_INTERVAL (5U * US_PER_SEC)

static Network network;
static MQTTClient client;
static unsigned char sendbuf[100], readbuf[100];

static void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.id = 0;
    message.payload = (void*)MQTT_STATE_MSG;
    message.payloadlen = strlen(MQTT_STATE_MSG);
    
    MQTTPublish(&client, MQTT_TOPIC, &message);
    printf("Published: %s\n", MQTT_STATE_MSG);
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    int rc;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_PORT);
    if (rc != SUCCESS) {
        printf("Could not connect to the network: %d\n", rc);
        return 1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.clientID.cstring = MQTT_CLIENT_ID;
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Could not connect to the MQTT broker: %d\n", rc);
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_periodic_wakeup(&last_wakeup, MSG_INTERVAL);
    }

    return 0;
}