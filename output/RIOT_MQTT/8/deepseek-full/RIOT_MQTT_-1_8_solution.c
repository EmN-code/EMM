
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <msg.h>
#include <thread.h>
#include <mutex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

static void network_init(void) {
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
}

static void mqtt_init(MQTTClient *client) {
    NetworkInit(&client->network);
    MQTTClientInit(client, &client->network, TIMEOUT, NULL, 0, NULL, 0);
}

static int mqtt_connect(MQTTClient *client) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    return MQTTConnect(client, &options);
}

static void mqtt_publish(MQTTClient *client) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);
    MQTTPublish(client, TOPIC, &message);
}

static void mqtt_disconnect(MQTTClient *client) {
    MQTTDisconnect(client);
}

int main(void) {
    MQTTClient client;
    network_init();
    mqtt_init(&client);

    if (mqtt_connect(&client) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        mqtt_publish(&client);
        xtimer_sleep(5);
    }

    mqtt_disconnect(&client);
    return 0;
}
