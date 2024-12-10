
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0

static MQTTClient mqtt_client;
static Network network;
static unsigned char sendbuf[200];
static unsigned char readbuf[200];

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
}

int main(void) {
    printf("Starting MQTT client...\n");

    NetworkInit(&network);
    MQTTClientInit(&mqtt_client, &network, MQTT_KEEPALIVE, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    IPStack ipstack;
    ipstack.connect = NetworkConnect;
    ipstack.read = NetworkReceive;
    ipstack.write = NetworkSend;
    ipstack.disconnect = NetworkDisconnect;

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;

    int rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return 1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
