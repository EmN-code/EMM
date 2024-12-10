#include <arpa/inet.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <thread.h>
#include <sys/socket.h>
#include <paho_mqtt.h>
#include <event/thread.h>
#include <net/sock/udp.h>
#include "MQTTClient.h"  // Required for MQTT functionality, problematic as blacklisted

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_PUB_TOPIC "state"
#define STACK_SIZE THREAD_STACKSIZE_MAIN
#define MAIN_QUEUE_SIZE (8)

static char stack[STACK_SIZE];
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *mqtt_pub_thread(void *arg) {
    (void)arg;
    int rc;
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];

    NetworkInit(&network);

    rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Error connecting to broker\n");
        return NULL;
    }

    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32Client";
    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("MQTT connect failed\n");
        return NULL;
    }

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = "work";
    message.payloadlen = strlen("work");

    while (1) {
        rc = MQTTPublish(&client, MQTT_PUB_TOPIC, &message);
        if (rc != 0) {
            printf("Error publishing\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5 seconds
    }

    NetworkDisconnect(&network);
    return NULL;
}

int main(void) {
    puts("MQTT example started");
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_pub_thread, NULL, "mqtt_pub_thread");

    return 0;
}