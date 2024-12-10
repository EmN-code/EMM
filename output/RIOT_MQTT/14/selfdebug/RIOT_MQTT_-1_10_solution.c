// This program connects to an MQTT broker and publishes a message every 5 seconds.

#include <string.h>
#include "net/sock/udp.h"
#include "mqtt.h"
#include "msg.h"
#include "thread.h"
#include "xtimer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define BUFFER_SIZE 128
#define MQTT_PUBLISH_INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t msg_queue[8];

static void *mqtt_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);

    Network network;
    MQTTClient client;
    unsigned char sendbuf[BUFFER_SIZE], readbuf[BUFFER_SIZE];

    NetworkInit(&network);
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, BUFFER_SIZE, readbuf, BUFFER_SIZE);

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)"work or not work";
    message.payloadlen = strlen(message.payload);

    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = "esp32_client";

    MQTTConnect(&client, &connect_data);

    while (1) {
        MQTTPublish(&client, "topic/state", &message);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}
