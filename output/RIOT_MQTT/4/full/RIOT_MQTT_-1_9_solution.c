#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/paho_mqtt.h"
#include "msg.h"
#include "mutex.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "assert.h"
#include "net/gcoap.h"
#include "MQTTClient.h"

#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define MQTT_PUB_TOPIC      "device/state"
#define MQTT_PUB_INTERVAL   (5U)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[256], recvbuf[256];

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    
    NetworkInit(&network);
    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Error: Could not connect to network.\n");
        return -1;
    }
    
    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32Client";
    
    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Error: Could not connect to MQTT broker.\n");
        return -1;
    }
    printf("Connected to MQTT broker.\n");
    
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void*)"work or not work";
        message.payloadlen = strlen("work or not work");

        rc = MQTTPublish(&client, MQTT_PUB_TOPIC, &message);
        if (rc != 0) {
            printf("Error: Could not publish message.\n");
        }
        else {
            printf("Message published: work or not work\n");
        }

        xtimer_sleep(MQTT_PUB_INTERVAL);
    }

    return 0;
}