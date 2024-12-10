
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/ip.h>
#include <net/ipv4/addr.h>
#include <net/ipv4/udp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void publish_state(void) {
    static const char *state = "work"; // or "not work" based on your logic
    static const char *topic = "state";

    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &remote, 0);

    char buf[64];
    int len = snprintf(buf, sizeof(buf), "{\"topic\":\"%s\",\"state\":\"%s\"}", topic, state);

    sock_udp_send(&sock, buf, len, &remote);
    sock_udp_close(&sock);
}

int main(void) {
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }
    return 0;
}
