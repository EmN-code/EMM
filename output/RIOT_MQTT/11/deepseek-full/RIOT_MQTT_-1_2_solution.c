
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <net/sock.h>
#include <net/af.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <sys/socket.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL 5

static void publish_state(MQTTClient client) {
    MQTTMessage message;
    message.qos = QOS1;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    int rc = MQTTClient_publishMessage(client, TOPIC, &message, NULL);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Message published\n");
    }
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, BROKER_IP, "RIOT_MQTT_CLIENT", MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return 1;
    }

    while (1) {
        publish_state(client);
        sleep(INTERVAL);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    sock_udp_close(&sock);

    return 0;
}
