#include "net/mqttsn.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static char mqtt_thread_stack[THREAD_STACKSIZE_DEFAULT];
static int running = 1;

void *mqtt_thread(void *args) {
    (void)args;
    uint8_t sendbuf[128], readbuf[128];
    MQTTClient client;
    Network network;

    MQTTClientInit(&client, &network, 0, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != 0) {
        printf("Failed to connect to network\n");
        return NULL;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    const char *topic = "state/status";
    const char *message = "work";
    MQTTMessage pubMessage;
    pubMessage.qos = QOS0;
    pubMessage.retained = 0;
    pubMessage.payload = (void *)message;
    pubMessage.payloadlen = strlen(message);

    xtimer_ticks32_t last_time = xtimer_now();

    while (running) {
        if (MQTTPublish(&client, topic, &pubMessage) != SUCCESS) {
            printf("Error while publishing\n");
        }
        xtimer_periodic_wakeup(&last_time, PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    puts("MQTT example");
    thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  mqtt_thread, NULL, "mqtt_thread");
    return 0;
}