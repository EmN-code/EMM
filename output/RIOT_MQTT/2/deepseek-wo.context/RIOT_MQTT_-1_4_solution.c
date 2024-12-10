
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
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static Network network;
static MQTTClient mqtt_client;

int mqtt_read(Network *n, unsigned char *read_buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_read(&n->sock, read_buffer, len);
}

int mqtt_write(Network *n, unsigned char *send_buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_write(&n->sock, send_buffer, len);
}

void mqtt_connect(void)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    int rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("MQTT connect failed, return code %d\n", rc);
    } else {
        printf("MQTT connected\n");
    }
}

void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    int rc = MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed, return code %d\n", rc);
    } else {
        printf("MQTT message published\n");
    }
}

int main(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, MQTT_BROKER_IP);

    sock_tcp_t sock;
    int res = sock_tcp_create(&sock, &local, &remote, SOCK_FLAGS_REUSE_EP);
    if (res < 0) {
        printf("Failed to create socket, error: %d\n", res);
        return 1;
    }

    network.sock = sock;
    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTClientInit(&mqtt_client, &network, 1000, NULL, 0, NULL, 0);

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
