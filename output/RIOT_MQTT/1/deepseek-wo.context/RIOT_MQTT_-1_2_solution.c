
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0
#define MQTT_ID 1
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT_MS 30000
#define MQTT_SEND_BUF_SIZE 1024
#define MQTT_READ_BUF_SIZE 1024
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_LARGE)

static char mqtt_send_buf[MQTT_SEND_BUF_SIZE];
static char mqtt_read_buf[MQTT_READ_BUF_SIZE];

static struct Network network;
static MQTTClient mqtt_client;

static int mqtt_connect(void)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;

    return MQTTConnect(&mqtt_client, &options);
}

static int mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = MQTT_DUP;
    message.id = MQTT_ID;
    message.payload = (void *)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    return MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
}

static void mqtt_thread(void)
{
    while (1) {
        if (mqtt_publish() != 0) {
            puts("Failed to publish message");
        }
        xtimer_sleep(5);
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTClientInit(&mqtt_client, &network, MQTT_COMMAND_TIMEOUT_MS,
                   mqtt_send_buf, MQTT_SEND_BUF_SIZE, mqtt_read_buf, MQTT_READ_BUF_SIZE);

    if (mqtt_connect() != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    thread_create(NULL, MQTT_THREAD_STACKSIZE, MQTT_THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
