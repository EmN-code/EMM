
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <xtimer.h>
#include <thread.h>
#include <mutex.h>
#include <msg.h>
#include <event/thread.h>
#include <net/sock/util.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT 1000
#define MQTT_SEND_BUF_SIZE 100
#define MQTT_READ_BUF_SIZE 100
#define MQTT_CLIENT_ID "esp32_client"

static char sendbuf[MQTT_SEND_BUF_SIZE];
static char readbuf[MQTT_READ_BUF_SIZE];

static void *mqtt_thread(void *arg)
{
    (void)arg;

    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, MQTT_COMMAND_TIMEOUT, (unsigned char*)sendbuf, MQTT_SEND_BUF_SIZE, (unsigned char*)readbuf, MQTT_READ_BUF_SIZE);

    connectData.clientID.cstring = MQTT_CLIENT_ID;
    connectData.keepAliveInterval = MQTT_KEEPALIVE;
    connectData.cleansession = 1;

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        NetworkDisconnect(&network);
        return NULL;
    }

    message.qos = MQTT_QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    while (1) {
        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return NULL;
}

int main(void)
{
    thread_t *mqtt_thread_handle;
    char mqtt_stack[THREAD_STACKSIZE_DEFAULT];

    mqtt_thread_handle = thread_create(mqtt_stack, sizeof(mqtt_stack),
                                      THREAD_PRIORITY_MAIN - 1, 0,
                                      mqtt_thread, NULL, "mqtt_thread");

    if (mqtt_thread_handle == NULL) {
        printf("Failed to create MQTT thread\n");
        return 1;
    }

    return 0;
}
