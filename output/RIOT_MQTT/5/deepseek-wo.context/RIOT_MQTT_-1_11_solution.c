
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0
#define MQTT_ID 1
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT 30000
#define MQTT_SEND_BUF_SIZE 1024
#define MQTT_READ_BUF_SIZE 1024
#define MQTT_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[MQTT_SEND_BUF_SIZE];
static unsigned char readbuf[MQTT_READ_BUF_SIZE];

static void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = MQTT_DUP;
    message.id = MQTT_ID;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
}

static void mqtt_connect(void)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.clientID.cstring = "riot_esp32";

    MQTTConnect(&mqtt_client, &options);
}

static void mqtt_init(void)
{
    MQTTClientInit(&mqtt_client, &mqtt_network, MQTT_COMMAND_TIMEOUT, sendbuf, MQTT_SEND_BUF_SIZE, readbuf, MQTT_READ_BUF_SIZE);

    int rc = NetworkConnect(&mqtt_network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    mqtt_connect();
}

int main(void)
{
    mqtt_init();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
