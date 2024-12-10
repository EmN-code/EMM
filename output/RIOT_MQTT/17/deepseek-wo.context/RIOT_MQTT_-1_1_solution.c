
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t sock;
static mqtt_client_t client;

static void _mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_addr_from_str(remote.addr.ipv4, BROKER_IP);

    int res = sock_tcp_connect(&sock, &remote, &local, 0);
    if (res < 0) {
        puts("Error connecting to broker");
        return;
    }

    mqtt_client_init(&client, (sock_tcp_t *)&sock);

    mqtt_connect_t conn = {
        .keep_alive = 60,
        .clean_session = 1,
        .client_id = "riot_esp32",
    };

    res = mqtt_connect(&client, &conn);
    if (res < 0) {
        puts("Error connecting to MQTT broker");
        return;
    }
}

static void _mqtt_publish(void)
{
    mqtt_publish_t pub = {
        .topic = TOPIC,
        .payload = PAYLOAD,
        .payload_len = strlen(PAYLOAD),
        .qos = MQTT_QOS_0,
        .retain = 0,
    };

    int res = mqtt_publish(&client, &pub);
    if (res < 0) {
        puts("Error publishing message");
    } else {
        puts("Message published successfully");
    }
}

int main(void)
{
    _mqtt_connect();

    while (1) {
        _mqtt_publish();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
