
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <mutex.h>
#include <timex.h>
#include <stdbool.h>
#include <MQTTClient.h>
#include <thread.h>
#include <msg.h>
#include <sys/socket.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 20
#define MQTT_QOS 1
#define MQTT_RETAIN false
#define MQTT_CLEAN_SESSION true

#define MSG_QUEUE_SIZE 10

typedef struct {
    sock_udp_t sock;
    sock_udp_ep_t local;
    sock_udp_ep_t remote;
} network_t;

static network_t network;
static MQTTClient mqtt_client;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void network_init(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;
    sock_udp_create(&network.sock, &local, NULL, 0);

    inet_pton(AF_INET, MQTT_BROKER_IP, &network.remote.addr.ipv4);
    network.remote.port = MQTT_BROKER_PORT;
    network.remote.family = AF_INET;
}

void mqtt_init(void) {
    MQTTClientInit(&mqtt_client, (Network*)&network, 1000, NULL, 0, NULL, 0);
}

int mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEAN_SESSION;
    return MQTTConnect(&mqtt_client, &options);
}

int mqtt_publish(const char *payload) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)payload;
    message.payloadlen = strlen(payload);
    return MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
}

void *mqtt_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        if (mqtt_publish("work") != 0) {
            puts("Failed to publish message");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    network_init();
    mqtt_init();

    if (mqtt_connect() != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    thread_t *mqtt_thread_id = thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                                             THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    msg_t msg;
    while (1) {
        msg_send(&msg, mqtt_thread_id);
        xtimer_sleep(5);
    }

    return 0;
}
