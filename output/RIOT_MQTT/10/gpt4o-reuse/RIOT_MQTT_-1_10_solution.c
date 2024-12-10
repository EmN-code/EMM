
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "net/sock/udp.h"
#include "net/mqtt.h"
#include "xtimer.h"

#define ADDRESS     "47.102.103.1"
#define PORT        1883
#define CLIENTID    "mqtt_riot_client"
#define TOPIC       "state"
#define PAYLOAD     "work"
#define QOS         1
#define TIMEOUT     1000000L

static mqtt_client_t client;
static sock_udp_ep_t broker = SOCK_IPV4_EP_ANY;
static char rx_buf[128];
static char tx_buf[128];

void mqtt_pub_handler(void *arg, mqtt_event_t event)
{
    (void)arg;
    if (event == MQTT_EVENT_PUBLISH_SENT) {
        printf("Message published\n");
    }
}

void mqtt_sub_handler(void *arg, mqtt_event_t event, const char *topic, void *data, size_t len)
{
    (void)arg;
    if (event == MQTT_EVENT_PUBLISH_RECEIVED) {
        printf("Message received on topic %s: %.*s\n", topic, (int)len, (char *)data);
    }
}

void mqtt_connect(void)
{
    broker.port = PORT;
    broker.family = AF_INET;
    broker.netif = SOCK_ADDR_ANY_NETIF;
    inet_pton(AF_INET, ADDRESS, &broker.addr.ipv4);

    mqtt_client_init(&client);
    mqtt_connect_params_t conn_params = {
        .keep_alive = 60,
        .client_id = CLIENTID,
        .clean_session = true,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = false,
        .username = NULL,
        .password = NULL,
        .client_addr = &broker,
        .transport = MQTT_TRANSPORT_UDP
    };

    if (mqtt_connect(&client, &conn_params) != MQTT_OK) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    printf("Connected to MQTT broker\n");
}

void mqtt_publish(void)
{
    mqtt_publish_params_t pub_params = {
        .message = {
            .topic = TOPIC,
            .payload = PAYLOAD,
            .payload_len = strlen(PAYLOAD),
            .qos = QOS,
            .retain = false
        },
        .handler = mqtt_pub_handler,
        .arg = NULL
    };

    if (mqtt_publish(&client, &pub_params) != MQTT_OK) {
        printf("Failed to publish message\n");
    }
}

int main(void)
{
    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
