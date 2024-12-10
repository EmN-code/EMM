
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t _sock;

static int _connect_to_broker(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_addr_from_str(BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&_sock, &remote, &local, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: failed to connect to broker");
        return -1;
    }

    return 0;
}

static int _publish_state(void)
{
    mqtt_client_t client;
    mqtt_connect_info_t conn_info = {
        .client_id = "riot_esp32",
        .keep_alive = 60,
        .clean_session = 1,
    };

    mqtt_client_init(&client, &_sock, NULL, 0, NULL, 0, NULL);

    if (mqtt_connect(&client, &conn_info) != 0) {
        puts("Error: failed to connect to MQTT broker");
        return -1;
    }

    mqtt_publish_info_t pub_info = {
        .topic = TOPIC,
        .payload = PAYLOAD,
        .payload_len = strlen(PAYLOAD),
        .qos = MQTT_QOS_0,
        .retain = 0,
    };

    if (mqtt_publish(&client, &pub_info) != 0) {
        puts("Error: failed to publish message");
        return -1;
    }

    mqtt_disconnect(&client);
    return 0;
}

int main(void)
{
    if (_connect_to_broker() < 0) {
        return 1;
    }

    while (1) {
        if (_publish_state() < 0) {
            break;
        }
        xtimer_usleep(INTERVAL);
    }

    sock_tcp_close(&_sock);
    return 0;
}
