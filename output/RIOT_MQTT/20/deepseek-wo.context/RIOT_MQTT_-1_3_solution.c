
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_CLEAN_SESSION 1
#define MQTT_CLIENT_ID "riot_esp32"

static MQTTClient mqttClient;
static Network network;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = MQTT_MESSAGE_LEN;

    MQTTPublish(&mqttClient, MQTT_TOPIC, &message);
}

int main(void) {
    printf("Starting MQTT example\n");

    NetworkInit(&network);
    MQTTClientInit(&mqttClient, &network, MQTT_KEEPALIVE, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = MQTT_CLIENT_ID;
    data.keepAliveInterval = MQTT_KEEPALIVE;
    data.cleansession = MQTT_CLEAN_SESSION;

    if (MQTTConnect(&mqttClient, &data) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
