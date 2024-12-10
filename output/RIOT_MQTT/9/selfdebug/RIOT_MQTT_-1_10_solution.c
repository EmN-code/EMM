#include <xtimer.h>
#include <mqtt_client.h>
#include <net/netdev.h>
#include <net/netdev/ieee802154.h>
#include <msg.h>
#include <stdio.h>
#include "net/network_layer.h"

#define MAIN_QUEUE_SIZE     (8)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

static MQTTClient client;
MQTTMessage message;
Network network;
unsigned char sendbuf[100], readbuf[100];

const char* addr = "47.102.103.1";
int port = 1883;

void mqtt_pub_task(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    const char *topic = "device/state";
    while (1) {
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)"work or not work";
        message.payloadlen = strlen(message.payload);

        if (MQTTPublish(&client, topic, &message) != SUCCESS) {
            printf("Error publishing message\n");
        }
        xtimer_periodic_wakeup(&last_wakeup, 5 * US_PER_SEC);
    }
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    NetworkInit(&network);
    if (NetworkConnect(&network, addr, port) != SUCCESS) {
        printf("Error connecting to broker\n");
        return 1;
    }

    MQTTClientInit(&client, &network, 10000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    char* client_id = "riot-esp32";

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = client_id;
    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, (thread_task_func_t)mqtt_pub_task,
                                     NULL, "mqtt_pub_thread");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Error creating MQTT pub thread\n");
        return 1;
    }

    return 0;
}