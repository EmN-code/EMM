#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/neighbors.h>
#include <net/sock/udp.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_TOPIC "device/state"
#define PUBLISH_INTERVAL (5U * MS_PER_SEC)

static void *publish_thread(void *arg) {
    (void)arg;
    MQTTClient client;
    Network network;
    unsigned char sendbuf[100];
    unsigned char readbuf[100];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    char *addr_ip = MQTT_BROKER_IP;
    int port_number = MQTT_BROKER_PORT;
    
    NetworkConnect(&network, addr_ip, port_number);

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_device";

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void*)"work";
        message.payloadlen = strlen("work");

        if (MQTTPublish(&client, PUBLISH_TOPIC, &message) != SUCCESS) {
            printf("Failed to publish message\n");
        } else {
            printf("Published: %s\n", (char*)message.payload);
        }

        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid;
    char publish_thread_stack[THREAD_STACKSIZE_MAIN];

    pid = thread_create(publish_thread_stack, sizeof(publish_thread_stack),
                        THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                        publish_thread, NULL, "publish thread");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Error: unable to create publish thread\n");
        return 1;
    }

    return 0;
}