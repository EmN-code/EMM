#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>

// Define a stack size
#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)

// Buffer and necessary variables for MQTT
unsigned char sendbuf[100];
unsigned char readbuf[100];
MQTTClient client;
Network network;

// The thread function to handle MQTT publishing
void *mqtt_thread(void *arg) {
    (void) arg;
    char* payload = "work";
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = payload;
        message.payloadlen = strlen(payload);

        MQTTPublish(&client, "state", &message);

        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Initialize the network and MQTT client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the MQTT broker
    NetworkConnect(&network, "47.102.103.1", 1883);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_ESP32";
    MQTTConnect(&client, &data);

    // Start the MQTT thread
    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    // We leave the main thread without purpose by sleeping indefinitely
    ztimer_sleep(ZTIMER_MSEC, 1000);

    return 0;
}