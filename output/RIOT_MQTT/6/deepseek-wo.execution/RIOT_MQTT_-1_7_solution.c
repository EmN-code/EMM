
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <thread.h>
#include <mutex.h>
#include <msg.h>
#include <event/thread.h>
#include <timex.h>
#include <ztimer.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT_MS 30000
#define MQTT_SEND_BUF_SIZE 1024
#define MQTT_READ_BUF_SIZE 1024

static void mqtt_publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTPublish(client, MQTT_TOPIC, &message);
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    char sendbuf[MQTT_SEND_BUF_SIZE];
    char readbuf[MQTT_READ_BUF_SIZE];

    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_udp_create(&network.my_socket, &local, &remote, 0);

    MQTTClientInit(&client, &network, MQTT_COMMAND_TIMEOUT_MS, sendbuf, MQTT_SEND_BUF_SIZE, readbuf, MQTT_READ_BUF_SIZE);

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_MQTT_CLIENT";
    connectData.keepAliveInterval = MQTT_KEEPALIVE;
    connectData.cleansession = 1;

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        mqtt_publish_state(&client);
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    sock_udp_close(&network.my_socket);

    return 0;
}
