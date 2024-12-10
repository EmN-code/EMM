#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "thread.h"

#define MQTT_TOPIC "device/status"
#define MQTT_MSG "work or not work"
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define INTERVAL (5U)  // 5 seconds

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *send_thread(void *arg) {
    (void)arg;
    while (1) {
        printf("Publishing to topic %s: %s\n", MQTT_TOPIC, MQTT_MSG);
        emcute_pub(&topic, MQTT_MSG, strlen(MQTT_MSG), EMCUTE_QOS_0);
        sleep(INTERVAL);
    }

    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &gw.addr) != 1) {
        puts("Error: unable to parse gateway address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, send_thread, NULL, "send_thread");

    // Main thread can perform other tasks here or enter a sleep loop
    while(1) {
        sleep(1);
    }

    return 0;
}