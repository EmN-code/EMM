#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define BUFFER_SIZE         128
#define PUBLISH_INTERVAL    5

void mqtt_publish(sock_udp_t *sock, sock_udp_ep_t *remote) {
    const char *topic = "device/state";
    const char *message = "work";
    char buffer[BUFFER_SIZE];
    int len = snprintf(buffer, BUFFER_SIZE, "PUBLISH %s %s", topic, message);

    sock_udp_send(sock, buffer, len, remote);
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = MQTT_BROKER_PORT };
    
    inet_pton(AF_INET6, MQTT_BROKER_ADDRESS, &remote.addr.ipv6);
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        mqtt_publish(&sock, &remote);
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}