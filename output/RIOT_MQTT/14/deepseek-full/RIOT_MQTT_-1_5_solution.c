
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <xtimer.h>
#include <thread.h>
#include <mutex.h>
#include <msg.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/sock/util.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define TIMEOUT 10000L

static volatile bool running = true;
static mutex_t mqtt_mutex = MUTEX_INIT;

void mqtt_publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(client, TOPIC, &message);
}

void *mqtt_thread(void *arg) {
    MQTTClient client;
    Network network;
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = "riot_esp32";
    connect_data.keepAliveInterval = 20;
    connect_data.cleansession = 1;

    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != 0) {
        printf("Failed to connect to network\n");
        return NULL;
    }

    if (MQTTConnect(&client, &connect_data) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    while (running) {
        mutex_lock(&mqtt_mutex);
        mqtt_publish_state(&client);
        mutex_unlock(&mqtt_mutex);
        xtimer_sleep(5);
    }

    MQTTClient_disconnect(&client, 10000);
    MQTTClient_destroy(&client);

    return NULL;
}

int main(void) {
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
