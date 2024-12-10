#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqttsn.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)  // 5 seconds

static sock_udp_ep_t gw = {
    .family = AF_INET,
    .port = SERVER_PORT
};

static const char *topic = "device/state";

static char message[] = "work";

void publish_state(sock_tcp_t *sock) {
    mqtt_packet_t pdu;
    uint8_t buf[128];
    size_t len;

    mqtt_sn_publish(&pdu, buf, sizeof(buf), topic, message, strlen(message));
    len = pdu.len;

    if (sock_udp_send(&sock->udp.sock, buf, len, &gw) < 0) {
        puts("Error sending message");
    } else {
        puts("Message sent");
    }
}

int main(void) {
    sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &remote.sin_addr.s_addr);

    sock_tcp_t sock;

    if (sock_tcp_connect(&sock, (const sock_addr_t *)&remote, sizeof(remote)) < 0) {
        puts("Unable to connect to broker\n");
        return 1;
    }

    xtimer_ticks32_t last = xtimer_now();

    while (1) {
        if (xtimer_now() - last > PUB_INTERVAL) {
            publish_state(&sock);
            last = xtimer_now();
        }
    }

    return 0;
}