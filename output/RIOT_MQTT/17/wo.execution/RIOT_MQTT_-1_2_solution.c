#include "thread.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdio.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define ADDRESS_IP "47.102.103.1"
#define PORT_NUMBER 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "device/state"
#define QOS 1
#define TIMEOUT 1000L

static char sendbuf[100];
static char readbuf[100];

void *publish_thread(void *arg) {
    Network network;
    MQTTClient client;
    int rc = 0;

    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if ((rc = NetworkConnect(&network, ADDRESS_IP, PORT_NUMBER)) != 0) {
        printf("Failed to connect to network, return code %d\n", rc);
        return NULL;
    }

    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    char payload[20];

    while (1) {
        sprintf(payload, "work");
        message.payload = payload;
        message.payloadlen = strlen(payload);

        if ((rc = MQTTPublish(&client, TOPIC, &message)) != 0) {
            printf("Failed to publish message, return code %d\n", rc);
        } else {
            printf("Message published: %s\n", payload);
        }

        sleep(5);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid;
    char stack[THREAD_STACKSIZE_DEFAULT];

    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, publish_thread, NULL, "publish_thread");

    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    return 0;
}