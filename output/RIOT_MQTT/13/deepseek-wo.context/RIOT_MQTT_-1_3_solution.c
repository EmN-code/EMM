
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
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
#define MQTT_TIMEOUT_MS 1000
#define MQTT_BUFFER_SIZE 1024
#define MQTT_STACK_SIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)
#define MQTT_THREAD_PRIO (THREAD_PRIORITY_MAIN - 2)
#define MQTT_KEEPALIVE_PING ((MQTT_KEEPALIVE / 4))

static void mqtt_publish(MQTTClient *client, const char *topic, const char *message)
{
    MQTTMessage msg;
    msg.payload = (void *)message;
    msg.payloadlen = strlen(message);
    msg.qos = MQTT_QOS;
    msg.retained = MQTT_RETAIN;
    msg.dup = 0;

    MQTTPublish(client, topic, &msg);
}

int main(void)
{
    sock_tcp_ep_t broker = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4);

    Network network;
    MQTTClient client;
    unsigned char sendbuf[MQTT_BUFFER_SIZE], readbuf[MQTT_BUFFER_SIZE];

    MQTTClientInit(&client, &network, MQTT_TIMEOUT_MS, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

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

    rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(5);
    }

    return 0;
}
