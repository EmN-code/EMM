#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <socket.h>
#include <stdlib.h>
#include <mbox.h>
#include <mutex.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <timex.h>
#include <thread.h>
#include <stdio.h>
#include <paho_mqtt.h>

// Define the server IP and port
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define STACK_SIZE_THREAD (THREAD_STACKSIZE_DEFAULT)

static char stack[STACK_SIZE_THREAD];

// MQTT client and network
MQTTClient client;
Network network;

// MQTT Buffers
unsigned char sendbuf[80], readbuf[80];

// Timer
msg_t msg_queue[8];
evtimer_msg_t evtimer;

// MQTT publish function
void* mqtt_thread(void *arg){
    (void)arg;

    char* topic = "mytopic";
    MQTTMessage message;
    message.qos = 1;
    message.retained = 0;
    message.dup = 0;

    char* payload = "Working";

    while(1){
        message.payload = (void*)payload;
        message.payloadlen = strlen(payload);

        int rc = MQTTPublish(&client, topic, &message);
        if(rc != 0){
            printf("Error publishing: %d\n", rc);
        }

        // Wait for 5 seconds
        evtimer_thread_timeout(&evtimer, 5 * US_PER_SEC);
    }
    return NULL;
}

int main(void){
    printf("MQTT client application\n");

    // Setup network
    NetworkInit(&network);
    ssize_t conn_res = NetworkConnect(&network, SERVER_IP, SERVER_PORT);
    if (conn_res < 0) {
        printf("Failed to connect to network\n");
        return 1;
    }

    // Initialize MQTT Client
    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = (char*) "mqtt_client";

    int rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return 1;
    }
    printf("Connected to MQTT broker\n");

    // Initialize the event timer
    evtimer_msg_init_queue(&evtimer, msg_queue, 8);

    // Create thread for MQTT publishing
    thread_create(stack, STACK_SIZE_THREAD, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  mqtt_thread, NULL, "mqtt");

    return 0;
}