#include "xtimer.h"
#include "stdio.h"
#include "net/emcute.h"
#include "net/socket.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define STATE_TOPIC "device/state"
#define STATE_MSG "work"
#define SLEEP_INTERVAL (5 * US_PER_SEC)

static int publish_state(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = MQTT_BROKER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, MQTT_BROKER_IP);

    sock_udp_t sock;
    sock_udp_create(&sock, &gw, NULL, 0);

    while (true) {
        size_t len = strlen(STATE_MSG);
        sock_udp_send(&sock, STATE_MSG, len, &gw);
        printf("Published state: %s\n", STATE_MSG);
        xtimer_sleep(5);
    }

    return 0;
}

int main(void) {
    printf("MQTT client example for ESP32...\n");
    publish_state();
    return 0;
}