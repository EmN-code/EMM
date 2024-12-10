#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/mqttsn.h>
#include <xtimer.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static mqtt_client_t client;
static char mqtt_buf[256];
static char topic_name[64];

static int publish_state(mqtt_client_t *client) {
    static mqtt_message_t message;
    message.qos = MQTT_QoS0;
    message.retained = 0;
    message.payload = "work";
    message.payloadlen = strlen("work");

    if (mqtt_publish(client, &message, topic_name) != 0) {
        printf("Failed to publish\n");
        return -1;
    }
    return 0;
}

int main(void) {
    strcpy(topic_name, TOPIC);
    mqtt_init(&client, CLIENT_ID, mqtt_buf, sizeof(mqtt_buf));

    sock_ip_ep_t server;
    server.family = AF_INET;
    server.netif = SOCK_ADDR_ANY_NETIF;
    server.port = SERVER_PORT;

    inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4);

    if (mqtt_connect(&client, &server) != 0) {
        printf("Could not connect to broker\n");
        return 1;
    }

    while (1) {
        publish_state(&client);
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}