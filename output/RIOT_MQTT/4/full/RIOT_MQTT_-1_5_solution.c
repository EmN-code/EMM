#include "dht_params.h"
#include "dht.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "timex.h"
#include "time_units.h"
#include "xtimer.h"
#include "ztimer.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "msg.h"
#include "thread.h"
#include "mutex.h"
#include "assert.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define PUBLISH_INTERVAL_S (5U)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

void mqtt_publish(const char *message) {
    MQTTMessage msg;
    msg.qos = QOS0;
    msg.retained = 0;
    msg.payload = (void *)message;
    msg.payloadlen = strlen(message);
    int rc = MQTTPublish(&client, MQTT_TOPIC, &msg);
    if (rc != SUCCESS) {
        printf("Error publishing message: %d\n", rc);
    }
}

void connect_and_publish(void) {
    int rc;
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "iot-device";
    
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to network: %d\n", rc);
        return;
    }
    
    rc = MQTTConnect(&client, &data);
    if (rc != SUCCESS) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return;
    }
    
    while (1) {
        mqtt_publish("work");
        ztimer_sleep(ZTIMER_SEC, PUBLISH_INTERVAL_S);
    }
}

int main(void) {
    printf("MQTT client application starting\n");

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    connect_and_publish();

    return 0;
}