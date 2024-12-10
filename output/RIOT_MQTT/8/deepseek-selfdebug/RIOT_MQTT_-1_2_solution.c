
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_WILL_FLAG 0
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_CLEAN_SESSION 1
#define MQTT_VERSION 3
#define MQTT_TIMEOUT 10000
#define MQTT_BUFFER_SIZE 1024
#define MQTT_STACK_SIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void mqtt_connect(void);
static void mqtt_publish(void);

static sock_tcp_t mqtt_sock;
static char mqtt_buffer[MQTT_BUFFER_SIZE];

int main(void)
{
    puts("MQTT client starting");

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}

static void mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    int res = sock_tcp_create(&mqtt_sock, &local, &remote, 0);
    if (res < 0) {
        puts("Error creating TCP socket");
        return;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEAN_SESSION;
    options.MQTTVersion = MQTT_VERSION;

    uint8_t connect_buf[MQTT_BUFFER_SIZE];
    int connect_len = MQTTSerialize_connect(connect_buf, sizeof(connect_buf), &options);
    if (connect_len <= 0) {
        puts("Error serializing connect packet");
        return;
    }

    ssize_t sent = sock_tcp_send(&mqtt_sock, connect_buf, connect_len, 0);
    if (sent != connect_len) {
        puts("Error sending connect packet");
        return;
    }

    uint8_t connack_buf[4];
    ssize_t received = sock_tcp_recv(&mqtt_sock, connack_buf, sizeof(connack_buf), SOCK_NO_TIMEOUT);
    if (received <= 0) {
        puts("Error receiving connack packet");
        return;
    }

    MQTTConnackData connack_data;
    int connack_len = MQTTDeserialize_connack(&connack_data, connack_buf, sizeof(connack_buf));
    if (connack_len <= 0 || connack_data.returnCode != 0) {
        puts("Error in connack packet");
        return;
    }

    puts("MQTT connected");
}

static void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    uint8_t publish_buf[MQTT_BUFFER_SIZE];
    int publish_len = MQTTSerialize_publish(publish_buf, sizeof(publish_buf), 0, 0, MQTT_TOPIC, &message);
    if (publish_len <= 0) {
        puts("Error serializing publish packet");
        return;
    }

    ssize_t sent = sock_tcp_send(&mqtt_sock, publish_buf, publish_len, 0);
    if (sent != publish_len) {
        puts("Error sending publish packet");
        return;
    }

    puts("MQTT message published");
}
