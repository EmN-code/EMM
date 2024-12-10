
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT 1000
#define MQTT_SEND_BUF_SIZE 256
#define MQTT_READ_BUF_SIZE 256
#define MQTT_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[MQTT_SEND_BUF_SIZE];
static unsigned char readbuf[MQTT_READ_BUF_SIZE];

static void mqtt_connect(void)
{
    NetworkInit(&mqtt_network);
    MQTTClientInit(&mqtt_client, &mqtt_network, MQTT_COMMAND_TIMEOUT, sendbuf, MQTT_SEND_BUF_SIZE, readbuf, MQTT_READ_BUF_SIZE);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.MQTTVersion = 3;

    int rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("MQTT connect failed\n");
    } else {
        printf("MQTT connected\n");
    }
}

static void mqtt_publish(const char *topic, const char *message)
{
    MQTTMessage pubmsg;
    pubmsg.qos = MQTT_QOS;
    pubmsg.retained = MQTT_RETAIN;
    pubmsg.payload = (void *)message;
    pubmsg.payloadlen = strlen(message);

    int rc = MQTTPublish(&mqtt_client, topic, &pubmsg);
    if (rc != 0) {
        printf("MQTT publish failed\n");
    } else {
        printf("MQTT published: %s\n", message);
    }
}

int main(void)
{
    printf("Starting MQTT example\n");

    sock_tcp_ep_t broker = SOCK_IPV4_EP_ANY;
    broker.port = MQTT_BROKER_PORT;
    broker.family = AF_INET;

    int sock = sock_tcp_create(&broker, NULL, SOCK_FLAGS_REUSE_EP);
    if (sock < 0) {
        printf("Failed to create TCP socket\n");
        return 1;
    }

    if (sock_tcp_connect(sock, &broker, SOCK_NO_TIMEOUT) < 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    mqtt_network.sock = sock;
    mqtt_connect();

    while (1) {
        mqtt_publish(MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(5);
    }

    return 0;
}
