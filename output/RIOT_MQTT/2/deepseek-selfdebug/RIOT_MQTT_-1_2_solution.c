
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"

static sock_udp_t sock;
static sock_udp_ep_t broker = SOCK_IPV4_EP_ANY;

void publish_state(void) {
    char message[20];
    strcpy(message, MQTT_MESSAGE);

    mqtt_publish_params_t pub_params = {
        .topic = MQTT_TOPIC,
        .payload = message,
        .payload_len = strlen(message),
        .qos = MQTT_QOS_0,
        .retain = false,
        .dup = false
    };

    mqtt_client_t client;
    mqtt_client_init(&client);

    mqtt_connect_params_t conn_params = {
        .client_id = MQTT_CLIENT_ID,
        .clean_session = true,
        .keep_alive = 60,
        .will = NULL,
        .username = NULL,
        .password = NULL
    };

    if (mqtt_connect(&client, &conn_params) == 0) {
        mqtt_publish(&client, &pub_params);
        mqtt_disconnect(&client);
    }
}

int main(void) {
    broker.port = MQTT_BROKER_PORT;
    broker.family = AF_INET;
    broker.netif = SOCK_ADDR_ANY_NETIF;

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4) <= 0) {
        puts("Error: invalid broker IP address");
        return 1;
    }

    if (sock_udp_create(&sock, NULL, &broker, 0) < 0) {
        puts("Error: failed to create UDP socket");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
