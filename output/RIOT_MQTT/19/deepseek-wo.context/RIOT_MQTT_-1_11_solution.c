
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 30
#define MQTT_CLEAN_SESSION 1

#define INTERVAL_MS (5 * 1000U)

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

void mqtt_network_read(Network *n, unsigned char *buf, int len, int timeout)
{
    (void)timeout;
    sock_tcp_recv(n->sock, buf, len, SOCK_NO_TIMEOUT);
}

void mqtt_network_write(Network *n, unsigned char *buf, int len, int timeout)
{
    (void)timeout;
    sock_tcp_send(n->sock, buf, len, 0);
}

void mqtt_connect(void)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEAN_SESSION;
    options.MQTTVersion = 3;

    if (MQTTConnect(&mqtt_client, &options) != 0) {
        puts("MQTT connect failed");
        return;
    }

    puts("MQTT connected");
}

void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    if (MQTTPublish(&mqtt_client, MQTT_TOPIC, &message) != 0) {
        puts("MQTT publish failed");
        return;
    }

    puts("MQTT published");
}

int main(void)
{
    xtimer_init();

    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&mqtt_network.sock, &local, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Failed to create TCP socket");
        return 1;
    }

    mqtt_network.mqttread = mqtt_network_read;
    mqtt_network.mqttwrite = mqtt_network_write;

    MQTTClientInit(&mqtt_client, &mqtt_network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(INTERVAL_MS / 1000);
    }

    return 0;
}
