
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

static sock_udp_t sock;
static mqtt_client_t mqtt_client;
static mqtt_connect_info_t conn_info;

void mqtt_publish(void) {
    mqtt_publish_t pub;
    pub.topic = TOPIC;
    pub.payload = PAYLOAD;
    pub.payload_len = strlen(PAYLOAD);
    pub.qos = MQTT_QOS_0;
    pub.retain = 0;
    pub.dup = 0;

    mqtt_publish(&mqtt_client, &pub);
}

void mqtt_connect(void) {
    memset(&conn_info, 0, sizeof(conn_info));
    conn_info.client_id = "riot_esp32";
    conn_info.keep_alive = 60;
    conn_info.clean_session = 1;

    mqtt_client_init(&mqtt_client);
    mqtt_client.socket = &sock;

    mqtt_connect(&mqtt_client, &conn_info);
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, BROKER_IP);

    sock_udp_create(&sock, &local, &remote, 0);

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
