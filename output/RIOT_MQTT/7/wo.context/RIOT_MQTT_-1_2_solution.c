#include <string.h>
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "net/emcute.h"
#include "MQTTClient.h"

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 1883
#define PUB_INTERVAL_SEC (5U)

static char mqtt_client_stack[THREAD_STACKSIZE_DEFAULT];
static MQTTClient client;
static Network network;
static unsigned char sendbuf[80], readbuf[80];
static char topic[] = "esp32/status";

void mqtt_publish(void) {
    MQTTMessage message;
    char payload[50];

    snprintf(payload, sizeof(payload), "work or not work");

    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    MQTTPublish(&client, topic, &message);
    printf("Published: %s\n", payload);
}

void *mqtt_thread(void *arg) {
    (void)arg;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, SERVER_IP, SERVER_PORT);
    if (rc != SUCCESS) {
        printf("NetworkConnect failed: %d\n", rc);
        return NULL;
    }

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("MQTTConnect failed: %d\n", rc);
        return NULL;
    }

    while (1) {
        mqtt_publish();
        ztimer_sleep(ZTIMER_SEC, PUB_INTERVAL_SEC);
    }

    return NULL;
}

int main(void) {
    thread_create(mqtt_client_stack, sizeof(mqtt_client_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  mqtt_thread, NULL, "mqtt_thread");

    return 0;
}