// Code to connect to an MQTT broker using MQTTClient library on ESP32 with RIOT

// Include necessary headers 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <periph/uart.h>
#include <mqtt_client.h>

// MQTT broker details
#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUB_TOPIC   "state"
#define MQTT_PUB_MESSAGE "work or not work"

static void *mqtt_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t remote = {
        .family = AF_INET,
        .addr.ipv4.u32[0] = byteorder_htonl(0x2F666701),  // broker IP in hexadecimal
        .port = MQTT_BROKER_PORT
    };

    mqtt_client_t client;
    mqtt_client_init(&client, &remote, MQTT_BROKER_PORT);

    while (1) {
        mqtt_publish(&client, MQTT_PUB_TOPIC, MQTT_PUB_MESSAGE, strlen(MQTT_PUB_MESSAGE), MQTTClient_QOS0);
        xtimer_sleep(5); // publish every 5 seconds
    }

    return NULL;
}

int main(void)
{
    // Start the MQTT thread
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), (THREAD_PRIORITY_MAIN - 1), 0, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}