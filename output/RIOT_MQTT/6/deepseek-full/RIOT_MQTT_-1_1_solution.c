
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t sock;
static mqtt_client_t client;
static char sendbuf[128];
static char readbuf[128];

void mqtt_connect(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_addr_from_str(remote.addr.ipv4, BROKER_IP);

    sock_tcp_create(&sock, &local, &remote, 0);
    mqtt_client_init(&client, &sock, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf), NULL);

    mqtt_connect_msg_t connect_msg = {
        .keep_alive = 60,
        .client_id = "riot_esp32",
        .clean_session = true,
    };

    mqtt_client_connect(&client, &connect_msg);
}

void mqtt_publish(void) {
    mqtt_publish_msg_t publish_msg = {
        .topic = TOPIC,
        .payload = PAYLOAD,
        .payload_len = strlen(PAYLOAD),
        .qos = MQTT_QOS_0,
        .retain = false,
    };

    mqtt_client_publish(&client, &publish_msg);
}

int main(void) {
    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
