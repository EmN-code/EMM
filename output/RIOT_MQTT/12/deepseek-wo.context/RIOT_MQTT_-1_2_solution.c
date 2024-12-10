
#include <stdio.h>
#include <string.h>
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
#define MQTT_CLEAN_SESSION 1
#define MQTT_TIMEOUT 10000

static Network network;
static MQTTClient mqttClient;

void mqtt_publish(MQTTClient *client, const char *topic, const char *message) {
    MQTTMessage msg;
    msg.qos = MQTT_QOS;
    msg.retained = MQTT_RETAIN;
    msg.dup = 0;
    msg.id = 0;
    msg.payload = (void *)message;
    msg.payloadlen = strlen(message);

    MQTTPublish(client, topic, &msg);
}

int main(void) {
    sock_tcp_ep_t broker = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4);

    sock_tcp_t sock;
    sock_tcp_create(&sock, NULL, &broker, 0);

    network.sock = sock;
    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;

    MQTTClientInit(&mqttClient, &network, MQTT_TIMEOUT, NULL, 0, NULL, 0);

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
        mqtt_publish(&mqttClient, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(5);
    }

    return 0;
}
