
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/tcp.h"
#include "net/ipv4/addr.h"
#include "net/mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1
#define MQTT_CLIENT_ID "esp32_client"

#define INTERVAL_MS (5 * 1000U)

static MQTTClient mqtt_client;
static sock_tcp_t tcp_sock;

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
}

int main(void) {
    xtimer_init();

    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&tcp_sock, &remote, &local, 0) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return 1;
    }

    MQTTClientInit(&mqtt_client, (Network *)&tcp_sock, 1000, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEAN_SESSION;

    if (MQTTConnect(&mqtt_client, &options) != 0) {
        puts("Error: failed to connect to MQTT broker");
        return 1;
    }

    puts("Connected to MQTT broker");

    while (1) {
        mqtt_publish();
        xtimer_sleep(INTERVAL_MS / 1000);
    }

    return 0;
}
