#include <net/sock/udp.h>
#include <net/sock.h>
#include <mqtt/MQTTClient.h>

#include "xtimer.h"
#include "thread.h"
#include "net/af.h"
#include "net/conn/udp.h"
#include "net/sock/udp.h"

#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PUB_INTERVAL 5

static MQTTClient client;
static Network network;
static unsigned char sendbuf[80], readbuf[80];
static char stack[THREAD_STACKSIZE_DEFAULT];

void publish_state(void) {
    MQTTMessage message;
    char *pubTopic = MQTT_TOPIC;

    char *payload = "work or not work";
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    while (1) {
        MQTTPublish(&client, pubTopic, &message);
        xtimer_sleep(MQTT_PUB_INTERVAL);
    }
}

int main(void) {
    int rc;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    NetworkInit(&network);

    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to network\n");
        return rc;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "ESP32Client";

    rc = MQTTConnect(&client, &data);
    if (rc != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return rc;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, publish_state, NULL, "publish_thread");

    return 0;
}