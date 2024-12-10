
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

#define MSG_QUEUE_SIZE (8)
#define MAIN_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];

static void mqtt_publish(void)
{
    static const char *message = "work";
    static int message_len = 4;

    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    mqtt_pkt_t pkt;
    mqtt_pkt_init(&pkt, 0, 0);
    mqtt_connect(&pkt, MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD, 0, 0, 0);
    mqtt_publish(&pkt, MQTT_TOPIC, message, message_len, 0, 0);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &remote, 0);
    sock_udp_send(&sock, pkt.data, pkt.length, &remote);
    sock_udp_close(&sock);
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
