
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

static sock_tcp_t sock;
static mqtt_client_t client;
static char sendbuf[128];
static char readbuf[128];

static void _mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    sock_tcp_create(&sock, &local, &remote, 0);
    mqtt_client_init(&client, &sock, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf), NULL);

    mqtt_connect_t conn = {
        .keep_alive = 60,
        .clean_session = 1,
        .client_id = "riot_esp32",
    };

    mqtt_connect(&client, &conn);
}

static void _mqtt_publish(const char *payload)
{
    mqtt_publish_t pub = {
        .topic = TOPIC,
        .payload = payload,
        .payload_len = strlen(payload),
        .qos = MQTT_QOS_0,
        .retain = 0,
    };

    mqtt_publish(&client, &pub);
}

int main(void)
{
    _mqtt_connect();

    bool state = true;

    while (1) {
        if (state) {
            _mqtt_publish(PAYLOAD_WORK);
        } else {
            _mqtt_publish(PAYLOAD_NOT_WORK);
        }

        state = !state;
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
