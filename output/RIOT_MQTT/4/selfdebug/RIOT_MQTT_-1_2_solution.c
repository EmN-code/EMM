#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <thread.h>
#include <mutex.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://47.102.103.1:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "state"
#define QOS         1
#define TIMEOUT     10000L

static char *status_message = "not work";

void thread_function(void) {
    Network network;
    unsigned char sendbuf[100], readbuf[100];
    int rc = 0, count = 0;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    Mutex mtx = MUTEX_INIT;

    NetworkInit(&network);
    rc = NetworkConnect(&network, ADDRESS, 1883);
    if (rc != 0) {
        printf("Failed to connect to network\n");
        return;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENTID;

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return;
    }
    printf("Connected\n");

    while(1) {
        rc = MQTTPublish(&client, TOPIC, status_message);
        if (rc != 0) {
            printf("Error while publishing: %d\n", rc);
        }
        status_message = (count % 2 == 0) ? "work" : "not work";
        count++;
        xtimer_sleep(5);
    }

    NetworkDisconnect(&network);
    printf("Disconnected\n");
}

int main(void) {
    thread_t thread;
    char stack[THREAD_STACKSIZE_MAIN];

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, thread_function, NULL, "pub_thread");

    while (1) {
        thread_yield();
    }

    return 0;
}