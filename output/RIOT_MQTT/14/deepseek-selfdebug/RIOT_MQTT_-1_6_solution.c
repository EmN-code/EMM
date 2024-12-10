
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"

static void mqtt_connect(void);
static void mqtt_publish(void);

static sock_tcp_t mqtt_sock;
static mqtt_client_t mqtt_client;

int main(void)
{
    puts("Starting MQTT example");

    /* Initialize the TCP socket */
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, MQTT_BROKER_IP);

    if (sock_tcp_create(&mqtt_sock, &local, &remote, 0) < 0) {
        puts("Error creating TCP socket");
        return 1;
    }

    /* Initialize the MQTT client */
    mqtt_client_init(&mqtt_client, &mqtt_sock);

    /* Connect to the MQTT broker */
    mqtt_connect();

    /* Publish state every 5 seconds */
    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}

static void mqtt_connect(void)
{
    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = 60,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL
    };

    if (mqtt_connect(&mqtt_client, &conn_info) != 0) {
        puts("Error connecting to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

static void mqtt_publish(void)
{
    mqtt_publish_info_t pub_info = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = strlen(MQTT_MESSAGE),
        .qos = 0,
        .retain = 0
    };

    if (mqtt_publish(&mqtt_client, &pub_info) != 0) {
        puts("Error publishing message");
        return;
    }

    puts("Message published");
}
