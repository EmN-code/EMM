
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_TIMEOUT 1000

static sock_tcp_t mqtt_sock;
static mqtt_client_t mqtt_client;
static char sendbuf[128];
static char readbuf[128];

void mqtt_connect(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_tcp_create(&mqtt_sock, &local, &remote, 0);
    mqtt_client_init(&mqtt_client, &mqtt_sock, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf), NULL);

    mqtt_connect_msg_t connect_msg = {
        .keep_alive = MQTT_KEEPALIVE,
        .client_id = MQTT_CLIENT_ID,
        .clean_session = 1,
        .will_flag = 0,
        .username = NULL,
        .password = NULL,
    };

    mqtt_client_connect(&mqtt_client, &connect_msg);
}

void mqtt_publish(void) {
    mqtt_publish_msg_t publish_msg = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = MQTT_MESSAGE_LEN,
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    mqtt_client_publish(&mqtt_client, &publish_msg);
}

int main(void) {
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("No network interface found");
        return 1;
    }

    while (netif_is_up(netif) == 0) {
        xtimer_sleep(1);
    }

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
