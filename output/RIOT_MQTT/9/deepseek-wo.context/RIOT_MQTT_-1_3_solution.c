
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_CLEAN_SESSION 1
#define MQTT_TIMEOUT 1000
#define MQTT_BUFFER_SIZE 1024
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_PUBLISH_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static MQTTClient mqtt_client;
static char mqtt_buffer[MQTT_BUFFER_SIZE];

static void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.id = 0;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = MQTT_MESSAGE_LEN;

    MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
}

static void mqtt_connect(void)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEAN_SESSION;
    options.username.cstring = MQTT_USERNAME;
    options.password.cstring = MQTT_PASSWORD;
    options.willFlag = (MQTT_WILL_TOPIC != NULL);
    options.will.topicName.cstring = MQTT_WILL_TOPIC;
    options.will.message.cstring = MQTT_WILL_MESSAGE;
    options.will.qos = MQTT_WILL_QOS;
    options.will.retained = MQTT_WILL_RETAIN;

    MQTTConnect(&mqtt_client, &options);
}

static void mqtt_init(void)
{
    Network network;
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTClientInit(&mqtt_client, &network, MQTT_TIMEOUT, mqtt_buffer, sizeof(mqtt_buffer), mqtt_buffer, sizeof(mqtt_buffer));

    mqtt_connect();
}

int main(void)
{
    mqtt_init();

    while (1) {
        mqtt_publish();
        xtimer_usleep(MQTT_PUBLISH_INTERVAL);
    }

    return 0;
}
