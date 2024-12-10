
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define INTERVAL_MS 5000000

static void _mqtt_publish(void)
{
    sock_udp_t sock;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    mqtt_pkt_t pkt;
    uint8_t buf[128];
    mqtt_pkt_init(&pkt, buf, sizeof(buf));

    mqtt_publish(&pkt, MQTT_TOPIC, MQTT_MESSAGE, MQTT_MESSAGE_LEN, 0, 0);

    if (sock_udp_send(&sock, pkt.data, pkt.length, &remote) < 0) {
        puts("Error sending MQTT message");
    } else {
        printf("Published: %s\n", MQTT_MESSAGE);
    }

    sock_udp_close(&sock);
}

int main(void)
{
    while (1) {
        _mqtt_publish();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
