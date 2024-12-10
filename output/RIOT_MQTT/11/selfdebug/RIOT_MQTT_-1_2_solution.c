#include "lwip/ip6_addr.h"
#include "net/ipv4/addr.h"
#include "kernel_defines.h"
#include "xtimer.h"
#include "stdlib.h"
#include "string.h"
#include "mqtt_client.h"
#include "mqtt_packet.h"
#include "mqtt_transport_tcp.h"
#include "stdio.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_mqtt_client"
#define TOPIC "device/state"
#define QOS 0
#define WORK_STATE "work"
#define NOT_WORK_STATE "not work"

// Task stack allocation
static char stack[THREAD_STACKSIZE_LARGE];

void *mqtt_publisher(void *arg) {
    (void)arg;
    mqtt_transport_tcp_t transport;
    mqtt_client_t client;
    mqtt_packet_connect_data_t data = MQTT_packet_connect_data_initializer;

    memset(&client, 0, sizeof(client));
    mqtt_transport_tcp_init(&transport);

    /* Set MQTT broker details */
    data.clientid.cstring = MQTT_CLIENT_ID;
    data.keepAliveInterval = 60;
    data.cleansession = 1;

    unsigned char sendbuf[80], readbuf[80];

    mqtt_client_init(&client, &transport, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    ipv4_addr_t broker_addr;
    ipv4_addr_from_str(&broker_addr, MQTT_BROKER_IP);
    mqtt_transport_tcp_connect(&transport, &broker_addr, MQTT_BROKER_PORT);

    mqtt_client_connect(&client, &data);

    while (1) {
        mqtt_packet_publish_t publish;
        publish.qos = QOS;
        publish.retained = 0;
        publish.dup = 0;
        publish.id = 0;
        publish.payload = WORK_STATE;
        publish.payloadlen = strlen(WORK_STATE);
        mqtt_packet_publish(&client, TOPIC, &publish);

        xtimer_sleep(5);
    }

    mqtt_client_disconnect(&client);
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_publisher, NULL, "mqtt_pub");
    return 0;
}
