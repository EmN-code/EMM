
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/tcp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_LARGE)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void mqtt_publish(MQTTClient *client) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.id = 0;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    int rc = MQTTPublish(client, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Message published\n");
    }
}

int main(void) {
    sock_tcp_t sock;
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    int res = sock_tcp_connect(&sock, &remote, &local, 0);
    if (res < 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", res);
        return 1;
    }

    MQTTClient client;
    NetworkInit(&client.network);
    client.network.sock = &sock;
    client.network.mqttread = sock_tcp_read;
    client.network.mqttwrite = sock_tcp_write;

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = MQTT_KEEPALIVE;
    data.cleansession = 1;

    res = MQTTConnect(&client, &data);
    if (res != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", res);
        return 1;
    }

    while (1) {
        mqtt_publish(&client);
        xtimer_sleep(5);
    }

    return 0;
}
