#include "xtimer.h"
#include "random.h"
#include "msg.h"
#include "net/emcute.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"

static char stack[THREAD_STACKSIZE_DEFAULT];

void *mqtt_thread(void *arg) {
    (void)arg;

    MQTTClient client;
    Network network;
    unsigned char sendbuf[100], readbuf[100];
    int rc;
    MQTTMessage message;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
        printf("Error: TCP connect failed\n");
        return NULL;
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32_Client";

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Error: MQTT connect failed\n");
        return NULL;
    }

    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)"work";
    message.payloadlen = strlen("work");

    while (1) {
        if ((rc = MQTTPublish(&client, MQTT_TOPIC, &message)) != 0) {
            printf("Error: MQTT publish failed\n");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}