#include <ztimer.h>
#include <thread.h>
#include <timex.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/mqttsn.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "work_status"
#define INTERVAL 5

static MQTTClient client;
static Network network;

void initialize_mqtt(void) {
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
}

void mqtt_publish(const char* message) {
    MQTTMessage mqtt_message;
    mqtt_message.qos = QOS0;
    mqtt_message.retained = 0;
    mqtt_message.payload = (void*) message;
    mqtt_message.payloadlen = strlen(message);

    MQTTPublish(&client, TOPIC, &mqtt_message);
}

void* mqtt_thread(void *arg) {
    (void)arg;
    while (1) {
        mqtt_publish("work");
        ztimer_sleep(ZTIMER_SEC, INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("MQTT publish example
");

    initialize_mqtt();

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
