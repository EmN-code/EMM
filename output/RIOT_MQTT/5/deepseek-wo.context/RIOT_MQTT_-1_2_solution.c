
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/tcp.h"
#include "net/mqtt.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLEANSESSION 1
#define MQTT_CLIENTID "riot_esp32"

static void mqtt_publish(MQTTClient *client, const char *topic, const char *message)
{
    MQTTMessage msg;
    msg.qos = MQTT_QOS;
    msg.retained = MQTT_RETAIN;
    msg.dup = 0;
    msg.id = 0;
    msg.payload = (void *)message;
    msg.payloadlen = strlen(message);

    MQTTPublish(client, topic, &msg);
}

int main(void)
{
    sock_tcp_t sock;
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, &local, 0) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return 1;
    }

    MQTTClient client;
    NetworkInit(&client.network);
    client.network.sock = &sock;

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = MQTT_CLIENTID;
    data.keepAliveInterval = MQTT_KEEPALIVE;
    data.cleansession = MQTT_CLEANSESSION;

    if (MQTTConnect(&client, &data) != 0) {
        puts("Error: failed to connect to MQTT broker");
        return 1;
    }

    while (1) {
        mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    sock_tcp_close(&sock);

    return 0;
}
