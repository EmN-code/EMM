
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <xtimer.h>
#include <thread.h>
#include <mutex.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <timex.h>
#include <msg.h>
#include <event/thread.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"
#define MQTT_QOS 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT_MS 30000
#define MQTT_SEND_BUF_SIZE 1024
#define MQTT_READ_BUF_SIZE 1024
#define MQTT_CLIENT_ID "riot_esp32"

static char mqtt_send_buf[MQTT_SEND_BUF_SIZE];
static char mqtt_read_buf[MQTT_READ_BUF_SIZE];

static void mqtt_publish_task(void)
{
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, MQTT_COMMAND_TIMEOUT_MS, (unsigned char*)mqtt_send_buf, MQTT_SEND_BUF_SIZE, (unsigned char*)mqtt_read_buf, MQTT_READ_BUF_SIZE);

    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = MQTT_CLIENT_ID;
    connect_data.keepAliveInterval = MQTT_KEEPALIVE;
    connect_data.cleansession = 1;

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    if (MQTTConnect(&client, &connect_data) != 0) {
        printf("Failed to connect to MQTT broker\n");
        NetworkDisconnect(&network);
        return;
    }

    message.qos = MQTT_QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    while (1) {
        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Message published: %s\n", MQTT_MESSAGE);
        }
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
}

int main(void)
{
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, mqtt_publish_task, NULL, "mqtt_publish_task");

    return 0;
}
