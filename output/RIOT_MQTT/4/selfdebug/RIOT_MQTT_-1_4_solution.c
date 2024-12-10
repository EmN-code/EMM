#include <xtimer.h>
#include <thread.h>
#include <mutex.h>
#include <timex.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)

static MQTTClient client;
static Network network;

void *mqtt_thread(void *arg) {
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)"work";
        message.payloadlen = 4;

        MQTTPublish(&client, "state", &message);
        xtimer_sleep(PUB_INTERVAL / US_PER_SEC); // sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    char mqtt_client_stack[THREAD_STACKSIZE_LARGE];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 5000, NULL, 0, NULL, 0);

    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != SUCCESS) {
        puts("Error: Could not connect to broker");
        return 1;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riotsample";
    rc = MQTTConnect(&client, &options);
    if (rc != SUCCESS) {
        puts("Error: Could not connect to broker with MQTT");
        return 1;
    }

    thread_create(mqtt_client_stack, sizeof(mqtt_client_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}