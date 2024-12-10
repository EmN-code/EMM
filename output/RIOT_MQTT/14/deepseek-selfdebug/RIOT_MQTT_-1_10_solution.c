
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t sock;
static sock_tcp_ep_t remote;

static void publish_state(void)
{
    char buf[64];
    int len = snprintf(buf, sizeof(buf), "PUBLISH %s %s\n", MQTT_TOPIC, MQTT_MESSAGE);

    if (sock_tcp_send(&sock, buf, len, 0) < 0) {
        puts("Failed to send data");
    } else {
        puts("State published");
    }
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, NULL, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Failed to create socket");
        return 1;
    }

    if (sock_tcp_connect(&sock, &remote, 0) < 0) {
        puts("Failed to connect to MQTT broker");
        sock_tcp_close(&sock);
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
    }

    sock_tcp_close(&sock);
    return 0;
}
