#include <net/emcute.h>
#include <net/netdev.h>
#include <net/gnrc/netif.h>
#include <net/gnrc.h>
#include <string.h>
#include <unistd.h>
#include "mqtt_client.h"
#include "my_network_utility.h"

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define PUBLISH_INTERVAL_US (5 * US_PER_SEC)

static char mqtt_sendbuf[512];
static char mqtt_readbuf[512];
static MQTTClient mqtt_client;
static Network mqtt_network;

void mqtt_message_handler(MessageData *data) {
    printf("Message arrived on topic %.*s: %.*s",
           data->topicName->lenstring.len, data->topicName->lenstring.data,
           data->message->payloadlen, data->message->payload);
}

static void *publisher(void *arg) {
    (void)arg;

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = "work";
    message.payloadlen = strlen("work");

    while (1) {
        int res = MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
        if (res != 0) {
            printf("Error while publishing: %d\n", res);
        }
        usleep(PUBLISH_INTERVAL_US);
    }
    return NULL;
}

int main(void) {
    mqtt_network.my_network_interface = MY_WIFI;
    NetworkConnect(&mqtt_network, MQTT_BROKER_ADDRESS, MQTT_BROKER_PORT);

    MQTTClientInit(&mqtt_client, &mqtt_network, 1000,
                   mqtt_sendbuf, sizeof(mqtt_sendbuf),
                   mqtt_readbuf, sizeof(mqtt_readbuf));

    MQTTPublish(&mqtt_client, MQTT_TOPIC, "Device Connected");

    pthread_t pub_thread;
    pthread_create(&pub_thread, NULL, publisher, NULL);

    while (1) {
        sleep(1);
    }

    return 0;
}