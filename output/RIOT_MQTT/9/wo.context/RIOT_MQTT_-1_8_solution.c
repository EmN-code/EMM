#include "thread.h"
#include "kernel.h"
#include "net/sock/udp.h"
#include "random.h"
#include "msg.h"
#include "xtimer.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5LU * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t msg_queue[8];

void publish_state(MQTTClient* client) {
    MQTTMessage message;
    char payload[50];
    snprintf(payload, sizeof(payload), "work");
    message.payload = payload;
    message.payloadlen = strlen(payload);
    message.qos = QOS0;
    message.retained = 0;
    MQTTPublish(client, "state", &message);
}

void *mqtt_thread(void *arg) {
    (void)arg;

    MQTTClient client;
    Network mqtt_network;

    xtimer_ticks32_t last = xtimer_now32();

    NetworkInit(&mqtt_network);
    NetworkConnect(&mqtt_network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTClientInit(&client, &mqtt_network, 1000, NULL, 0, NULL, 0);

    while (1) {
        publish_state(&client);
        xtimer_periodic_wakeup(&last, PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_thread, NULL, "mqtt");

    return 0;
}