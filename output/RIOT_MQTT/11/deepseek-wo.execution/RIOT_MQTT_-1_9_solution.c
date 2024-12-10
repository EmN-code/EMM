
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <MQTTClient.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_COMMAND_TIMEOUT_MS 10000
#define MQTT_SEND_BUF_SIZE 1024
#define MQTT_READ_BUF_SIZE 1024

static Network network;
static MQTTClient mqttClient;
static unsigned char sendbuf[MQTT_SEND_BUF_SIZE];
static unsigned char readbuf[MQTT_READ_BUF_SIZE];

void connectToMQTTBroker(void) {
    NetworkInit(&network);
    MQTTClientInit(&mqttClient, &network, MQTT_COMMAND_TIMEOUT_MS, sendbuf, MQTT_SEND_BUF_SIZE, readbuf, MQTT_READ_BUF_SIZE);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&mqttClient, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    printf("Connected to MQTT broker\n");
}

void publishState(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    if (MQTTPublish(&mqttClient, MQTT_TOPIC, &message) != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Message published: %s\n", MQTT_PAYLOAD);
    }
}

int main(void) {
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = EMQX_SERVER_PORT;

    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    if (NetworkConnect(&network, EMQX_SERVER_IP, EMQX_SERVER_PORT) != 0) {
        printf("Failed to connect to network\n");
        return 1;
    }

    connectToMQTTBroker();

    while (1) {
        publishState();
        xtimer_sleep(5);
    }

    NetworkDisconnect(&network);
    MQTTDisconnect(&mqttClient);

    return 0;
}
