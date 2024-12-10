
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void publish_state(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    char buf[100];
    int len = snprintf(buf, sizeof(buf), "PUBLISH %s %s\n", TOPIC, PAYLOAD);

    if (sock_udp_send(&sock, buf, len, &remote) < 0) {
        puts("Error sending UDP packet");
    } else {
        puts("State published");
    }

    sock_udp_close(&sock);
}

int main(void) {
    puts("MQTT state publisher started");

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
