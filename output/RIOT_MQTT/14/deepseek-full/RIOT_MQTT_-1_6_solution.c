
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <thread.h>
#include <mutex.h>
#include <net/sock.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <mbox.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define KEEP_ALIVE 20

static mutex_t mqtt_mutex = MUTEX_INIT;
static Network network;
static MQTTClient client;

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&client, TOPIC, &message);
}

void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = KEEP_ALIVE;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    printf("Connected to MQTT broker\n");
}

void network_connect(void) {
    NetworkInit(&network);
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != 0) {
        printf("Failed to connect to network\n");
        return;
    }

    printf("Network connected\n");
}

void mqtt_init(void) {
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
}

void periodic_publish(void) {
    while (1) {
        mutex_lock(&mqtt_mutex);
        publish_state();
        mutex_unlock(&mqtt_mutex);
        xtimer_sleep(5);
    }
}

int main(void) {
    network_connect();
    mqtt_init();
    mqtt_connect();

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  (thread_task_func_t)periodic_publish, NULL, "periodic_publish");

    return 0;
}
