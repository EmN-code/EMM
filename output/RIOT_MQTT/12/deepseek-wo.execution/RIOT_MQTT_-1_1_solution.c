
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

static void _mqtt_connect(void)
{
    mqtt_connect_info_t conn_info = {
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

    mqtt_client_init(&client, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    mqtt_client_connect(&client, BROKER_IP, BROKER_PORT, &conn_info);
}

static void _mqtt_publish(const char *payload)
{
    mqtt_publish_info_t pub_info = {
        .topic = TOPIC,
        .payload = payload,
        .payload_len = strlen(payload),
        .qos = 0,
        .retain = 0
    };

    mqtt_client_publish(&client, &pub_info);
}

int main(void)
{
    // Initialize network interface
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("No network interface found");
        return 1;
    }

    // Connect to MQTT broker
    _mqtt_connect();

    // Publish state every 5 seconds
    while (1) {
        _mqtt_publish(PAYLOAD_WORK);
        xtimer_usleep(INTERVAL);
        _mqtt_publish(PAYLOAD_NOT_WORK);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
