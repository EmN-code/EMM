#include <unistd.h>
#include <stdbool.h>
#include <architecture.h>
#include <atomic.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <msg.h>
#include <mutex.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <netinet/in.h>
#include <paho_mqtt.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <unistd.h>
#include <xtimer.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define STACK_SIZE THREAD_STACKSIZE_DEFAULT
#define PRIORITY THREAD_PRIORITY_MAIN - 1
#define MESSAGE_BUFFER_SIZE 128

static char stack[STACK_SIZE];
static char mqtt_topic[] = "device/state";
static MQTTClient client;
static Network network;
static unsigned char sendbuf[80], readbuf[80];

void mqtt_publish_task(void *arg) {
    (void)arg;
    int rc;

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "client_id";

    NetworkInit(&network);
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return;
    }

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;

    for (;;) {
        char payload[] = "work";
        message.payload = payload;
        message.payloadlen = strlen(payload);

        rc = MQTTPublish(&client, mqtt_topic, &message);
        if (rc != 0) {
            printf("Failed to publish, return code %d\n", rc);
        }
        sleep(5);
    }

    NetworkDisconnect(&network);
}

int main(void) {
    thread_create(stack, sizeof(stack), PRIORITY, 0, mqtt_publish_task, NULL, "MQTT_Pub_Thread");
    return 0;
}