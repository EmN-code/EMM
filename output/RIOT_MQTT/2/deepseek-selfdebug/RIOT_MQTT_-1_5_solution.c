
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_THREAD_PRIORITY            (THREAD_PRIORITY_MAIN - 1)
#define MQTT_THREAD_STACKSIZE           (THREAD_STACKSIZE_LARGE)
#define EMQX_SERVER_IP                  "47.102.103.1"
#define EMQX_SERVER_PORT                1883
#define MQTT_TOPIC                      "state"
#define MQTT_MESSAGE                    "work"
#define MQTT_KEEPALIVE                  30
#define MQTT_QOS                        0
#define MQTT_RETAIN                     0
#define MQTT_PUBLISH_INTERVAL           (5 * 1000000U) /* 5 seconds in microseconds */

static sock_tcp_t _sock;
static MQTTClient _client;
static Network _network;

static int _mqtt_read(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_read(&n->sock, buffer, len, 0);
}

static int _mqtt_write(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_write(&n->sock, buffer, len, 0);
}

static void _mqtt_connect(void)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.MQTTVersion = 3;

    if (MQTTConnect(&_client, &options) != 0) {
        puts("Error: MQTT connect failed");
        return;
    }

    puts("MQTT connected");
}

static void _mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    if (MQTTPublish(&_client, MQTT_TOPIC, &message) != 0) {
        puts("Error: MQTT publish failed");
        return;
    }

    puts("MQTT message published");
}

static void _mqtt_thread(void)
{
    _network.mqttread = _mqtt_read;
    _network.mqttwrite = _mqtt_write;

    if (sock_tcp_connect(&_sock, EMQX_SERVER_IP, EMQX_SERVER_PORT, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: TCP connect failed");
        return;
    }

    puts("TCP connected");

    MQTTClientInit(&_client, &_network, 1000, NULL, 0, NULL, 0);
    _mqtt_connect();

    while (1) {
        _mqtt_publish();
        xtimer_usleep(MQTT_PUBLISH_INTERVAL);
    }
}

int main(void)
{
    thread_create(NULL, MQTT_THREAD_STACKSIZE, MQTT_THREAD_PRIORITY, 0,
                  (thread_task_func_t)_mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
