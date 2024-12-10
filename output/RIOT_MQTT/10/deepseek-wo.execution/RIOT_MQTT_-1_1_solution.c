
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/af.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD_WORK "work"
#define PAYLOAD_NOT_WORK "not work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t client;
static uint8_t sendbuf[128];
static uint8_t readbuf[128];

void publish_state(mqtt_client_t *client, const char *payload)
{
    mqtt_publish_params_t pub_params = {
        .topic = TOPIC,
        .payload = payload,
        .payload_len = strlen(payload),
        .qos = MQTT_QOS_0,
        .retain = 0,
        .dup = 0
    };

    mqtt_publish(client, &pub_params);
}

int main(void)
{
    // Initialize network interface
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("No network interface found");
        return 1;
    }

    // Initialize MQTT client
    mqtt_client_init(&client, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Failed to create UDP socket");
        return 1;
    }

    mqtt_connect_params_t conn_params = {
        .client_id = "riot_esp32",
        .keep_alive = 60,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL
    };

    if (mqtt_connect(&client, &conn_params) < 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    // Publish state every 5 seconds
    bool state = true;
    while (1) {
        const char *payload = state ? PAYLOAD_WORK : PAYLOAD_NOT_WORK;
        publish_state(&client, payload);
        state = !state;
        xtimer_usleep(INTERVAL);
    }

    // Disconnect from MQTT broker
    mqtt_disconnect(&client);

    // Close UDP socket
    sock_udp_close(&sock);

    return 0;
}
