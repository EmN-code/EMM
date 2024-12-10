#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqttsn.h>
#include <net/netif.h>
#include <stdio.h>
#include <string.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)
#define CLIENT_ID "esp32_client"
#define PUB_TOPIC "device/state"

static sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };

static void publish_work_state(void) {
    static const char* state = "work";
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &gw, 0);
    sock_udp_send(&sock, state, strlen(state), &gw);
    sock_udp_close(&sock);
}

int main(void) {
    netif_t *netif = NULL;
    netif = netif_iter(NULL);
    if (netif == NULL) {
        printf("[ERROR] No network interface found\n");
        return 1;
    }

    ipv6_addr_t addr;
    if (ipv6_addr_from_str(&addr, BROKER_IP) == NULL) {
        printf("[ERROR] Failed to parse broker IP\n");
        return 1;
    }
    memcpy(&gw.addr, &addr, sizeof(addr));

    printf("[INFO] Connecting to MQTT broker at %s:%d\n", BROKER_IP, BROKER_PORT);

    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, PUB_INTERVAL);
        publish_work_state();
    }

    return 0;
}