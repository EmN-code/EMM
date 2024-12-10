
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/sock/util.h>
#include <net/ipv4/addr.h>
#include <net/ipv6/addr.h>
#include <net/sock.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_LARGE)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char stack[MQTT_THREAD_STACKSIZE];
static Network network;
static MQTTClient mqttClient;

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTPublish(&mqttClient, MQTT_TOPIC, &message);
}

void *mqtt_thread(void *arg) {
    (void)arg;

    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    int result;

    local.port = 0;
    remote.port = MQTT_BROKER_PORT;
    remote.family = AF_INET;

    result = inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return 1;
    }

    NetworkInit(&network);
    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;

    if (MQTTConnect(&mqttClient, &options) != 0) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    thread_create(stack, sizeof(stack), MQTT_THREAD_PRIORITY, 0, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
