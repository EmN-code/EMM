#include <string.h>
#include <stdio.h>
#include "thread.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include "msg.h"
#include "net/mqttsn.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define KEEP_ALIVE 60
#define PUB_TOPIC "device/state"
#define PUB_MESSAGE "work"
#define PUB_INTERVAL (5U * US_PER_SEC) // Publish every 5 seconds

static sock_udp_t sock;
static sock_udp_ep_t gw_ep;

void publish_state(void) {
    uint8_t pub_buf[128];
    mqttsn_pub_t pub;
    pub.topic_name = PUB_TOPIC;
    pub.msg = PUB_MESSAGE;
    pub.msg_len = strlen(PUB_MESSAGE);
    mqttsn_publish(&sock, &pub);
    printf("Published state: %s\n", PUB_MESSAGE);
}

int main(void) {
    msg_init_queue(NULL, 0);

    gw_ep.family = AF_INET;
    gw_ep.netif = SOCK_ADDR_ANY_NETIF;
    ipv4_addr_from_str((ipv4_addr_t *)&gw_ep.addr.ipv4, SERVER_ADDR);
    gw_ep.port = SERVER_PORT;

    mqttsn_init(&sock, &gw_ep, CLIENT_ID, KEEP_ALIVE);

    while (1) {
        publish_state();
        xtimer_usleep(PUB_INTERVAL);
    }

    return 0;
}