#include <ztimer.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <thread.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "ESP32Client"
#define TOPIC "state"
#define QOS 1
#define TIMEOUT 10000L
#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];

void message_arrived(MessageData *data) {
    (void) data;
}

static void *mqtt_thread(void *arg) {
    (void)arg;
    int rc;
    MQTTClient client;
    Network network;
    unsigned char sendbuf[100], readbuf[100];

    // Initialize network
    NetworkInit(&network);
    rc = NetworkConnect(&network, SERVER_IP, SERVER_PORT);
    if (rc != 0) {
        printf("Error: Failed to connect to the network.\n");
        return NULL;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;
    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("MQTT connect failed with code %d\n", rc);
        return NULL;
    }
    printf("MQTT connected\n");

    rc = MQTTSubscribe(&client, TOPIC, QOS, message_arrived);
    if (rc != 0) {
        printf("MQTT subscribe failed with code %d\n", rc);
        return NULL;
    }

    MQTTMessage message;
    char *payload = "work";
    message.qos = QOS;
    message.retained = 0;

    while (1) {
        message.payload = (void *)payload;
        message.payloadlen = strlen(payload);
        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != 0) {
            printf("MQTT publish failed with code %d\n", rc);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), PRIORITY, 0, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}