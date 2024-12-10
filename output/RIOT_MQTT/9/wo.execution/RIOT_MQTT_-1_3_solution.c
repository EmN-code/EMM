#include "net/sock/util.h"
#include "net/gcoap.h"
#include "stdlib.h"
#include "stdio.h"
#include "sys/socket.h"
#include "net/emcute.h"
#include "netinet/in.h"
#include "paho_mqtt.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "riot-esp32"
#define TOPIC "esp32/status"
#define MESSAGE "{"state": "work"}"
#define STACK_SIZE (THREAD_STACKSIZE_MAIN)

static char stack[STACK_SIZE];
static MQTTClient client;
static Network network;

void *publish_thread(void *arg) {
    (void)arg;
    int rc;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    if ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
        return NULL;
    }

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)MESSAGE;
        message.payloadlen = strlen(MESSAGE);

        MQTTClient.publish(&client, TOPIC, &message);
        sleep(5);
    }

    return NULL;
}

int main(void) {
    unsigned char sendbuf[256], readbuf[256];

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, publish_thread, NULL, "publish_thread");

    return 0;
}