
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_LARGE)

static MQTTClient mqtt_client;
static char mqtt_client_id[32];
static char mqtt_payload[64];

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.id = 0;
    message.payload = (void*)mqtt_payload;
    message.payloadlen = strlen(mqtt_payload);

    MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
}

void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.clientID.cstring = mqtt_client_id;

    MQTTConnect(&mqtt_client, &options);
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);

    mqtt_client.socket = &sock;
    mqtt_client.isConnected = 0;

    snprintf(mqtt_client_id, sizeof(mqtt_client_id), "riot-esp32-%04x", (unsigned int)xtimer_now_usec());

    mqtt_connect();

    while (1) {
        strcpy(mqtt_payload, "work");
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
