#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <thread.h>
#include <timex.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/status"
#define MQTT_CLIENT_ID "esp32_riot_client"
#define MQTT_MSG "work"
#define MQTT_BUFFER_SIZE 100
#define PUBLISH_INTERVAL 5  // seconds

static char mqtt_send_buf[MQTT_BUFFER_SIZE];
static char mqtt_read_buf[MQTT_BUFFER_SIZE];

static MQTTClient client;
static Network network;

void on_message(MessageData *data) {
    printf("Received message on topic %.*s: %.*s\n",
           (int)data->topicName->lenstring.len,
           data->topicName->lenstring.data,
           (int)data->message->payloadlen,
           (char *)data->message->payload);
}

static int mqtt_thread(void *arg) {
    (void)arg;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    NetworkInit(&network);
    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network.\n");
        return -1;
    }

    MQTTClientInit(&client, &network, 2000,
                   (unsigned char *)mqtt_send_buf, MQTT_BUFFER_SIZE,
                   (unsigned char *)mqtt_read_buf, MQTT_BUFFER_SIZE);

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker.\n");
        return -1;
    }
    printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)MQTT_MSG;
    message.payloadlen = strlen(MQTT_MSG);

    while (1) {
        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message.\n");
        } else {
            printf("Message published.\n");
        }
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return 0;
}

int main(void) {
    thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN - 1,
                  0, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}