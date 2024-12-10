#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "net/emcute.h"
#include "net/mqtt.h"
#include "net/sock/udp.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static mqtt_client_t client;
static mqtt_message_t message;
static sock_udp_ep_t remote;

void *mqtt_thread(void *arg) {
    (void)arg; // Unused
    while (1) {
        message.payload = (void *)"work";
        message.payloadlen = strlen("work");
        message.qos = MQTT_QOS_0;
        message.retained = 0;
        mqtt_publish(&client, TOPIC, &message);
        usleep(5000000); // 5 seconds
    }
    return NULL;
}

int main(void) {
    msg_init_queue(mqtt_buf, ARRAY_SIZE(mqtt_buf));
    memset(&client, 0, sizeof(client));
    memcpy(remote.addr.ipv4, ip_addr, sizeof(ip_addr));
    remote.port = SERVER_PORT;

    mqtt_connect(&client, CLIENT_ID, &remote);
    thread_create(stack, sizeof(stack), PRIORITY, 0, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}