
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

static void mqtt_publish(void)
{
    static uint8_t sendbuf[100];
    static uint8_t recvbuf[100];
    struct Network network;
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;

    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_tcp_t sock;
    sock_tcp_create(&sock, &local, &remote, SOCK_FLAGS_REUSE_EP);

    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.username.cstring = MQTT_USERNAME;
    options.password.cstring = MQTT_PASSWORD;

    int rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("MQTT connect failed\n");
        return;
    }

    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    rc = MQTTPublish(&client, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed\n");
    }

    MQTTDisconnect(&client);
    sock_tcp_close(&sock);
}

int main(void)
{
    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
